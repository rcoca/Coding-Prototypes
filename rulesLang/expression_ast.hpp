#ifndef EXPRESSION_AST_HPP
#define EXPRESSION_AST_HPP
#include <string>
#include <vector>
#include <boost/variant.hpp>


struct expression_ast
{
    struct nil{};
    struct class_decl;
    struct classes_decl;
    struct rule_group;
    struct program_expr;
    struct and_expr;
    struct or_expr;
    struct not_expr;
    struct mofn_expr;
    struct expression;

    struct rule_def;
    struct seq_expr;
    struct func_call;

    struct array;

    struct class_decl
    {
        std::string              name;
        std::vector<std::string> enums;
    };
    struct classes_decl
    {
        typedef class_decl value_type;
        std::vector<class_decl> declarations;
    };

    struct array
    {
        std::string name;
        uint begin;
        uint end;
    };

    struct identifier
    {
        boost::variant<array,std::string> expr;
    };
    struct func_call
    {
        std::string func;
        identifier  arg;
    };
    typedef
    boost::variant<
    nil // can't happen!
    , class_decl
    , classes_decl
    , identifier
    , boost::recursive_wrapper<func_call>
    , boost::recursive_wrapper<seq_expr>
    , boost::recursive_wrapper<and_expr>
    , boost::recursive_wrapper<or_expr>
    , boost::recursive_wrapper<not_expr>
    , boost::recursive_wrapper<mofn_expr>
    , boost::recursive_wrapper<expression>
    , boost::recursive_wrapper<program_expr>
    , boost::recursive_wrapper<rule_def>
    >
    type;


    struct inputs_decl
    {
        typedef identifier value_type;
        std::vector<identifier> identifiers;
    };
    struct outputs_decl
    {
        typedef identifier value_type;
        std::vector<identifier> identifiers;
    };
    struct expression
    {
        type expr;
    };

    struct and_expr
    {
        std::vector<expression> terms;
    };
    struct or_expr
    {
        std::vector<expression> terms;
    };
    struct not_expr
    {
        expression term;
    };
    struct mofn_expr
    {
        uint m;
        std::vector<expression> terms;
    };
    struct seq_expr
    {
        std::vector<expression> terms;
    };
    struct rule_def
    {
        identifier                target;
        std::vector<expression>   precedents;

    };
    struct rule_group
    {
        std::string group_id;
        inputs_decl inputs;
        outputs_decl outputs;
        bool         dense_flag;
        std::vector<rule_def>    rules;
    };
    struct program_expr
    {
        classes_decl classes;
        std::vector<rule_group> rule_groups;
        std::vector<std::string> rules_instances;
    };

    type generic_expression;
    expression_ast():generic_expression(type()){}
    expression_ast(const type& t):generic_expression(t){}
};
#endif // EXPRESSION_AST_HPP
