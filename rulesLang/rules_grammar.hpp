#ifndef RULES_GRAMMAR_HPP
#define RULES_GRAMMAR_HPP
#define BOOST_SPIRIT_DEBUG

#define BOOST_SPIRIT_UNICODE

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <boost/fusion/adapted.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_utree.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>

#include "expression_ast.hpp"
#include "expression_ast_print.hpp"
#include "fusion_expression_ast.hpp"


using namespace boost::spirit;


template< typename Iterator >
struct comment_skipper
        : qi::grammar< Iterator >
{

    comment_skipper()
        : comment_skipper::base_type(skipper_, "comment skipper")
    {

        skipper_ =('#' >> *(boost::spirit::qi::char_ - boost::spirit::qi::eol) >> boost::spirit::qi::eol | boost::spirit::qi::space);

    }

private :

    typename comment_skipper::base_type::start_type skipper_;

};

template <typename Iterator,typename Skipper>
struct rules_grammar : qi::grammar<Iterator,expression_ast::program_expr(),Skipper>
{
    rules_grammar()
        : rules_grammar::base_type(program)
    {
        using boost::spirit::qi::alpha;
        using boost::spirit::qi::alnum;


        using boost::spirit::qi::lexeme;
        using boost::spirit::qi::eol;
        using boost::spirit::qi::lit;
        using boost::spirit::qi::bool_parser;
        using boost::spirit::qi::bool_;

        using qi::on_error;
        using qi::fail;
        using qi::_val;
        using boost::phoenix::construct;
        using boost::phoenix::val;
        using boost::spirit::ascii::space_type;
        using boost::spirit::qi::omit;


        classes_values  %=  (lexeme[+(qi::char_('_')|qi::char_('+')|
                                     qi::char_('-')|qi::char_('.')|
                                     alnum)] % ',') >eps;

        class_id        %=  lexeme[(alpha>> *(qi::char_('_') | alnum))]>eps;
        class_decl      %=   (class_id > lit(':') > (
                                (qi::char_('{')> classes_values > qi::char_('}'))|
                                classes_values
                             )
                             > lit(';'))>eps ;


        classes_decl    %=   (lit("classes") > '{' > +class_decl > '}')>eps;

        group_id        %=  lexeme[(alpha>> *(qi::char_('_') | alnum))];
        ident           %=  lexeme[(alpha>> *(qi::char_('_') | alnum))];

        array           %=  lexeme[(alpha>> *(qi::char_('_') | alnum ))]>>'['>qi::uint_>"..">qi::uint_>']'>eps;

        identifier      %=  (array|ident);

        identseq        %=  (identifier % qi::lit(','));
        inputs_decl     %=  qi::lit("inputs")>>qi::lit(":")>identseq>';';
        outputs_decl    %=  qi::lit("outputs")>qi::lit(":")>identseq>';';
        dense_flag      %=  qi::lit("dense")>qi::lit(":")>qi::bool_>';';



        func_trans      %=lexeme[(alpha>> *(qi::char_('_') | alnum))]
                            >>qi::lit('(')>identifier>qi::lit(')');

        and_expr        %=  lit("and")>'('>seq_expr>')';
        or_expr         %=  lit("or")>'('>seq_expr>')';
        not_expr        %=  lit("~")>expression;
        mofn_expr       %=  qi::uint_>lit("of")>'{'>seq_expr>'}';

        expression      %=  (and_expr|or_expr|not_expr|mofn_expr|func_trans|identifier)>eps;
        seq_expr        %=  (expression % qi::lit(','));
        //Seq_expr        %=  (expression % qi::lit(','));
        rule_def        %=  identifier>lit(':')>lit('-')>seq_expr>';'>eps;
        //rule_def        %=  identifier>lit(':')>lit('-')>Seq_expr>';'>eps;
        rule_group      %=  lit("rules")>group_id>'{'
                >inputs_decl>outputs_decl>dense_flag>+rule_def
                >'}';


        rgroup_insts    %=   group_id % ','>';';
        program         %=   classes_decl>+rule_group>rgroup_insts;
#if 0
        BOOST_SPIRIT_DEBUG_NODE(identseq);
        BOOST_SPIRIT_DEBUG_NODE(classes_values);
        BOOST_SPIRIT_DEBUG_NODE(program);
        BOOST_SPIRIT_DEBUG_NODE(rgroup_insts);
        BOOST_SPIRIT_DEBUG_NODE(rule_group);
        BOOST_SPIRIT_DEBUG_NODE(rule_def);
        BOOST_SPIRIT_DEBUG_NODE(seq_expr);
        BOOST_SPIRIT_DEBUG_NODE(expression);
        BOOST_SPIRIT_DEBUG_NODE(mofn_expr);
        BOOST_SPIRIT_DEBUG_NODE(not_expr);
        BOOST_SPIRIT_DEBUG_NODE(or_expr);
        BOOST_SPIRIT_DEBUG_NODE(and_expr);
        BOOST_SPIRIT_DEBUG_NODE(identifier);
        BOOST_SPIRIT_DEBUG_NODE(outputs_decl);
        BOOST_SPIRIT_DEBUG_NODE(inputs_decl);
        BOOST_SPIRIT_DEBUG_NODE(classes_decl);
        BOOST_SPIRIT_DEBUG_NODE(class_decl);
        BOOST_SPIRIT_DEBUG_NODE(class_id);
#endif

        rule_group.name("rule_group");
        class_decl.name("class_decl");
        classes_decl.name("classes decl");
        array.name("array");
        ident.name("ident");
        identifier.name("identifier");
        identseq.name("identseq");
        seq_expr.name("seq_expr");
        class_id.name("class_id");
        classes_values.name("classes_values");

        inputs_decl.name("inputs_decl");
        outputs_decl.name("outputs_decl");
        dense_flag.name("dense_flag");
        and_expr.name("and_expr");
        or_expr .name("or_expr");
        not_expr.name("not_expr");
        mofn_expr .name("mofn_expr");
        expression.name("expression");
        rule_def.name("rule_def");
        rule_group.name("rule_group");
        program .name("program");

        rgroup_insts.name("rgroup_insts");

        on_error<fail>
                (
                    program
                    , std::cout
                    << val("program Error: Expecting ")
                    << _4                               // what failed?
                    << val(" here: \"")
                    << construct<std::string>(_3, _2)   // iterators to error-pos, end
                    << val("\"")
                    << std::endl
                    );


    }
    qi::rule<Iterator,std::string(),Skipper>                    class_id;
    qi::rule<Iterator,std::vector<std::string>(),Skipper>       classes_values;
    qi::rule<Iterator,std::string(),Skipper>                    group_id;
    qi::rule<Iterator,std::string(),Skipper>                    ident;

    qi::rule<Iterator,expression_ast::array(),Skipper>         array;
    qi::rule<Iterator,expression_ast::identifier(),Skipper>    identifier;
    qi::rule<Iterator,expression_ast::func_call(),Skipper>     func_trans;

    qi::rule<Iterator,std::vector<expression_ast::identifier>(),Skipper>    identseq;

    qi::rule<Iterator,expression_ast::inputs_decl(),Skipper>     inputs_decl;
    qi::rule<Iterator,expression_ast::outputs_decl(),Skipper>    outputs_decl;
    qi::rule<Iterator,bool(),Skipper>                            dense_flag;

    qi::rule<Iterator,expression_ast::and_expr(),Skipper>        and_expr;
    qi::rule<Iterator,expression_ast::or_expr(),Skipper>         or_expr;
    qi::rule<Iterator,expression_ast::not_expr(),Skipper>        not_expr;
    qi::rule<Iterator,expression_ast::mofn_expr(),Skipper>       mofn_expr;
    qi::rule<Iterator,expression_ast::expression(),Skipper>      expression;
    qi::rule<Iterator,std::vector<expression_ast::expression>(),Skipper>    seq_expr;
    //qi::rule<Iterator,expression_ast::seq_expr(),Skipper>       Seq_expr;
    qi::rule<Iterator,expression_ast::rule_def(),Skipper>       rule_def;


    qi::rule<Iterator,expression_ast::class_decl(),Skipper>     class_decl;

    qi::rule<Iterator,expression_ast::classes_decl(),Skipper>   classes_decl;
    qi::rule<Iterator,expression_ast::rule_group(),Skipper>     rule_group;
    qi::rule<Iterator,std::vector<std::string>(),Skipper>       rgroup_insts;
    qi::rule<Iterator,expression_ast::program_expr(),Skipper>   program;

};
#endif // RULES_GRAMMAR_HPP
