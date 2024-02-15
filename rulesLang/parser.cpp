#define BOOST_SPIRIT_DEBUG
#define BOOST_SPIRIT_UNICODE
#include <string>
#include <boost/optional.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/qi.hpp>
#include "parser.hpp"
#include "rules_grammar.hpp"

using namespace boost::spirit;

using input_type = std::string;
using base_iterator_type =  std::string::iterator ;
template struct comment_skipper< base_iterator_type >;
using skipper_type = comment_skipper< base_iterator_type >;

boost::optional<expression_ast> parse_rules(std::string s)
{
    expression_ast tree;
    rules_grammar<std::string::iterator,skipper_type> rules_parser;
    std::string::iterator last=s.begin();
    bool success = boost::spirit::qi::phrase_parse(last, s.end(), rules_parser,skipper_type(),tree);
    if(success && last==s.end())
    {
        std::cerr<< " Compile ok." << std::endl;
        return boost::optional<expression_ast>(tree);
    }
    else
    {
        std::cerr << "Error" << std::endl;
        std::string context(last, s.end());
        std::cerr << "stopped at: "<<std::endl << context << std::endl;
        return boost::optional<expression_ast>();
    }
}
