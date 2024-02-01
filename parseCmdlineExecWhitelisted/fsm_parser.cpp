#include <iostream>
#include <string>
#include <boost/regex.hpp>
#include <boost/process.hpp>
#include <boost/process/shell.hpp>
#include <boost/process/environment.hpp>
#include <boost/process/system.hpp>
//c++ -ggdb -O0 -std=c++17 fsm_parser.cpp -o /tmp/fsmParser -lboost_regex -lboost_system -lpthread
enum class STATE
{
    START=0,
    EXEC,
    ARGS,
    SSEQ,
    SGROUP,
    EGROUP,

    END=-1
};
std::map<STATE,std::string> state_names{
    {STATE::START,"start"},
    {STATE::EXEC,"exec"},
    {STATE::ARGS,"args"},
    {STATE::SSEQ,"sseq"},
    {STATE::SGROUP,"s_group"},
    {STATE::EGROUP,"e_group"},
    {STATE::END,"end"}
};
struct transition_t
{
    STATE src, dst;
    std::string condition;
    std::function<bool (std::string const&)> callback;
};

struct Parser
{
    Parser();
    typedef std::map<STATE,std::vector<transition_t>> fsm_t;
    bool run(std::string const& cmdline);

    std::map<STATE,std::vector<transition_t>> ParserFSM;
    std::stack<std::string> groupDelimStack;
    std::vector<std::string> execCollection;
};
Parser::Parser()
{
    std::function<bool (std::string const&)> trueCB=
            [](std::string const& s)->bool {return true;};
    std::function<bool (std::string const&)> collectExe=
            [this](std::string const& s){
        std::string exe_path=boost::process::search_path(s).string();
        std::cerr<<s<< "  in path: "<<exe_path<<std::endl;
        if(!exe_path.empty())
            this->execCollection.push_back(s);
        return true;
    };
    std::function<bool (std::string const&)> checkPushGroupSep=
            [this](std::string const& s){
        if(!groupDelimStack.empty() && s==groupDelimStack.top())
        {
            groupDelimStack.pop();
            return true;
        };
        return false;
    };
    std::function<bool (std::string const&)> pushGroupSep=
            [this](std::string const& s){
        groupDelimStack.push(s);
        //execCollection.push_back(s);
        return true;
    };

    //TODO: adjust callbacks
    //TODO: better filter regex matches. and match all the way to next expr
    std::string exec_detect{"^\\s*([A-Za-z0-9_\\-\\+\\.]+)"},
    args_detect{"^\\s*([^\\s;]+)"},
    sseq_detect{"^\\s*([;\\|])"},
    egroup_detect{"^\\s*(\"|'|`)"},
    sgroup_detect{"^\\s*(\"|'|`)"},
    empty_detect,
    end_detect{"^\\s*($)"};


    ParserFSM={
        {STATE::START,{
             {STATE::START,STATE::EXEC,exec_detect,collectExe},
             {STATE::START,STATE::SSEQ,sseq_detect,trueCB}, //=>EXEC
             {STATE::START,STATE::EGROUP,egroup_detect,checkPushGroupSep},
             {STATE::START,STATE::SGROUP,sgroup_detect,pushGroupSep}
         }
        },
        {STATE::SSEQ,{
            {STATE::SSEQ,STATE::EXEC,exec_detect,collectExe},
         }
        },
        {STATE::SGROUP,{
             {STATE::SGROUP,STATE::START,empty_detect,trueCB},
         },
        },        
        {STATE::EXEC,{             
             {STATE::EXEC,STATE::SSEQ,sseq_detect,trueCB},
             {STATE::EXEC,STATE::EGROUP,egroup_detect,checkPushGroupSep},
             {STATE::EXEC,STATE::SGROUP,sgroup_detect,pushGroupSep},
             {STATE::EXEC,STATE::ARGS,args_detect,trueCB},
             {STATE::EXEC,STATE::END,end_detect,trueCB}
         }
        },
        {STATE::ARGS,{
             {STATE::ARGS,STATE::EGROUP,egroup_detect,checkPushGroupSep},
             {STATE::ARGS,STATE::SGROUP,sgroup_detect,pushGroupSep},
             {STATE::ARGS,STATE::ARGS,args_detect, trueCB},
             {STATE::ARGS,STATE::SSEQ,sseq_detect,trueCB}, //=> START->EXEC
             {STATE::ARGS,STATE::END,end_detect,trueCB}
         },
        },
        {STATE::EGROUP,{
             {STATE::EGROUP,STATE::START,empty_detect,trueCB},
             {STATE::EGROUP,STATE::END,end_detect,trueCB}
         },
        }
    };
}

bool Parser::run(std::string const& cmdline)
{
    STATE state=STATE::START;
    std::string input(cmdline);
    while(state!=STATE::END && !input.empty()) //and still cmdline left
    {
        STATE srcstate=state;
        //use regex matches to advance. print each match and re-capture
        std::vector<transition_t> transitions=ParserFSM.at(state);
        boost::regex_constants::syntax_option_type flags = boost::regex::extended;
        bool transited=false;
        for(auto trans:transitions)
        {
            std::string matched;
            std::string captured;
            std::size_t matchLen=0;
            if(!trans.condition.empty())
            {
                boost::regex condition(trans.condition,flags);
                boost::smatch match;
                std::cerr<<"try:"<<state_names[trans.dst]<<":"<<trans.condition<<"] ["<<input<<std::endl;
                if(boost::regex_search(input,match,condition))
                {
                    matched.assign(match[0].first,match[0].second);
                    captured.assign(match[1].first,match[1].second);
                    matchLen=match.length();
                    std::cerr<<"\t matched:"<<matched<<" captured:"<<captured<<std::endl;
                    if(!trans.callback(captured)) continue;
                    state=trans.dst;
                    input=std::string(input.begin()+matchLen,input.end());
                    transited=true;
                    break;                    
                }
            }
            else
            {
                std::cerr<<"try default:"<<state_names[trans.dst]<<":"<<trans.condition<<"] ["<<input<<std::endl;
                std::cerr<<"\t matched:"<<matched<<" captured:"<<captured<<std::endl;
                trans.callback(captured);
                state=trans.dst;
                transited=true;
            }
        }
        if(!transited)
        {
            std::cerr<<"no transition, exiting: "<<state_names.at(state)<<" "<<input<<std::endl;
            return input.empty();
            break;
        }

    }
    std::cerr<<"end parsing: "<<state_names[state]<<"left:"<<input<<"]"<<std::endl;
    if(state==STATE::END || input.empty())
        return true;
    else
        return false;
}


void strip_ws(std::string& str)
{
    std::stringstream trimmer;
    trimmer << str;
    str.clear();
    trimmer >> str;
}

std::vector<std::string> tokenizer( const std::string& arguments, char delim )
{
    std::vector<std::string> tokens;
    std::stringstream   mySstream( arguments );
    std::string         temp;

    while( getline( mySstream, temp, delim ) ) {
        tokens.push_back( temp );
    }

    return tokens;
}

int main(int argc, char* argv[])
{
    const char* Ewlist    = ::getenv("cmdsWhitelist");

    std::string wlist;
    if(Ewlist) wlist=Ewlist;
    auto whiteList=tokenizer(wlist,',');
    for(auto& t:whiteList) strip_ws(t);
    std::set<std::string> whiteSet(whiteList.begin(),whiteList.end());

    boost::system::error_code ec;
    
    std::string args;

    for(int i=1;i<argc;++i)
    {
        args+=argv[i];
        if(i<argc-1)args+=" ";
    }
    Parser parser;
    bool parseResult=parser.run(args);
    std::cerr<<"parse result: "<<std::boolalpha<<parseResult<<std::endl;
    for(auto const& exe:parser.execCollection) std::cerr<<exe<<" "
                        <<std::boolalpha<<(whiteSet.find(exe)!=whiteSet.end())
                        <<std::endl;
    std::cerr<<std::endl;
    return 0;
}
