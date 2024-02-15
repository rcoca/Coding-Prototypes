#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP
#define BOOST_SPIRIT_DEBUG
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

        using qi::on_error;
        using qi::fail;
        using qi::_val;
        using boost::phoenix::construct;
        using boost::phoenix::val;
        using boost::spirit::ascii::space_type;
        using boost::spirit::qi::omit;


        classes_values  %=  lexeme[+(qi::char_('_')|qi::char_('+')|qi::char_('-') | alnum)] % ','>eps;
        class_id        %=  lexeme[(alpha>> *(qi::char_('_') | alnum))]>eps;
        class_decl      %=   class_id > lit(':') >  classes_values > lit(';')>eps ;
        classes_decl    %=   (lit("classes") > '{' > +class_decl > '}')>eps;

        ident           %=   lexeme[(alpha>> *(qi::char_('_') | alnum))]
                                >>*(lit('/')>qi::int_) >eps;

        array           %=   lexeme[(alpha>> *(qi::char_('_') | alnum ))]
                                >>'['>qi::uint_>"..">qi::uint_>']'
                                >>*(lit('/')>qi::int_) >eps;

        identifier      %=   (array|ident)>eps;
        ident_sequence  %=   identifier % ',';

        input           %=   (qi::lit("inputs")>>':'>>ident_sequence >';') ;
        output          %=   qi::lit("outputs")>>':'>>ident_sequence >';';

        and_expr        %=   qi::lit("and")>'('>expression % ','>')';
        or_expr         %=   qi::lit("or")>'('>expression % ',' >')' ;
        not_expr        %=   qi::lit('~')>expression;
        mofn_expr       %=   (qi::uint_>"of">"{">seq_expr>"}");

        seq_expr        %=   expression % ',';
        expression      %=   and_expr|or_expr|not_expr|mofn_expr|identifier;

        rule_decl       %=   identifier > ":-" > seq_expr >';';
        program         %=   classes_decl>input>output>+rule_decl;

        class_id.name("class id");
        classes_values.name("classes enum");
        class_decl.name("class decl");
        classes_decl.name("classes_decl");
        program.name("program");
        input.name("inputs:");output.name("outputs:");
        rule_decl.name("rule_decl");
        ident.name("term");array.name("array");
        identifier.name(" identifier");
        ident_sequence.name(" identifier_sequence");

        and_expr.name("and_expr");or_expr.name("or_expr");not_expr.name("not_expr");
        seq_expr.name("seq_expr");mofn_expr.name("MofN_expr");

        expression.name("expression");
        program.name("program");


        BOOST_SPIRIT_DEBUG_NODE(classes_decl);
        BOOST_SPIRIT_DEBUG_NODE(input);
        BOOST_SPIRIT_DEBUG_NODE(output);
        BOOST_SPIRIT_DEBUG_NODE(ident_sequence);

        BOOST_SPIRIT_DEBUG_NODE(ident);
        BOOST_SPIRIT_DEBUG_NODE(array);

        BOOST_SPIRIT_DEBUG_NODE(rule_decl);
        BOOST_SPIRIT_DEBUG_NODE(seq_expr);
        BOOST_SPIRIT_DEBUG_NODE(expression);
        BOOST_SPIRIT_DEBUG_NODE(and_expr);
        BOOST_SPIRIT_DEBUG_NODE(or_expr);
        BOOST_SPIRIT_DEBUG_NODE(mofn_expr);
        BOOST_SPIRIT_DEBUG_NODE(not_expr);
        BOOST_SPIRIT_DEBUG_NODE(seq_expr);
        BOOST_SPIRIT_DEBUG_NODE(program);
        on_error<fail>
                (
                    classes_decl
                    , std::cout
                    << val("classes_decl Error: Expecting ")
                    << _4                               // what failed?
                    << val(" here: \"")
                    << construct<std::string>(_3, _2)   // iterators to error-pos, end
                    << val("\"")
                    << std::endl
                    );
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
    qi::rule<Iterator,std::string(),Skipper> class_id;
    qi::rule<Iterator,std::vector<std::string>(),Skipper> classes_values;
    qi::rule<Iterator,expression_ast::class_decl(),Skipper> class_decl;
    qi::rule<Iterator,expression_ast::classes_decl(),Skipper> classes_decl;
    qi::rule<Iterator,expression_ast::program_expr(),Skipper> program;


    qi::rule<Iterator,expression_ast::inputs_expr(),Skipper> input;
    qi::rule<Iterator,expression_ast::outputs_expr(),Skipper> output;
    qi::rule<Iterator,expression_ast::array(),Skipper> array;
    qi::rule<Iterator,expression_ast::term(),Skipper> ident;
    qi::rule<Iterator,expression_ast::implies_expr(),Skipper> rule_decl;

    qi::rule<Iterator,expression_ast::identifier(),Skipper> identifier;
    qi::rule<Iterator,expression_ast::ident_sequence(),Skipper> ident_sequence;

    qi::rule<Iterator,expression_ast::and_expr(),Skipper> and_expr;
    qi::rule<Iterator,expression_ast::or_expr(),Skipper> or_expr;
    qi::rule<Iterator,expression_ast::and_subset_expr(),Skipper> mofn_expr;
    qi::rule<Iterator,expression_ast::seq_expr(),Skipper> seq_expr;
    qi::rule<Iterator,expression_ast::not_expr(),Skipper> not_expr;
    qi::rule<Iterator,expression_ast::type(),Skipper> expression;



};


#endif // GRAMMAR_HPP
