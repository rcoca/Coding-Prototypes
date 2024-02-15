#ifndef FUSION_EXPRESSION_AST_HPP
#define FUSION_EXPRESSION_AST_HPP
#include <string>
#include <vector>
#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/variant.hpp>
#include "expression_ast.hpp"

typedef boost::variant<expression_ast::array,std::string> identifier_expr_t;
BOOST_FUSION_ADAPT_STRUCT(
        expression_ast::identifier,
        (identifier_expr_t, expr)
)
BOOST_FUSION_ADAPT_STRUCT(
        expression_ast::func_call,
        (std::string, func)
        (expression_ast::identifier, arg)
)

BOOST_FUSION_ADAPT_STRUCT(
        expression_ast::class_decl,
        (std::string, name)
        (std::vector<std::string>,  enums)
)
BOOST_FUSION_ADAPT_STRUCT(
        expression_ast::classes_decl,
        (std::vector<expression_ast::class_decl>,declarations)
)


BOOST_FUSION_ADAPT_STRUCT(
        expression_ast::rule_def,
        (expression_ast::identifier, target)
        ( std::vector<expression_ast::expression>,   precedents)
)

BOOST_FUSION_ADAPT_STRUCT(
        expression_ast::inputs_decl,
        (std::vector<expression_ast::identifier>, identifiers)
)
BOOST_FUSION_ADAPT_STRUCT(
        expression_ast::outputs_decl,
        (std::vector<expression_ast::identifier>, identifiers)
)
BOOST_FUSION_ADAPT_STRUCT(
        expression_ast::program_expr,
        (expression_ast::classes_decl,              classes)
        (std::vector<expression_ast::rule_group>,   rule_groups)
        (std::vector<std::string>,                  rules_instances)
)

BOOST_FUSION_ADAPT_STRUCT(
        expression_ast::seq_expr,
        (std::vector<expression_ast::expression>, terms)
)

BOOST_FUSION_ADAPT_STRUCT(
        expression_ast::and_expr,
        (std::vector<expression_ast::expression>, terms)
)
BOOST_FUSION_ADAPT_STRUCT(
        expression_ast::or_expr,
        (std::vector<expression_ast::expression>, terms)
)
BOOST_FUSION_ADAPT_STRUCT(
        expression_ast::not_expr,
        (expression_ast::expression, term)
)
BOOST_FUSION_ADAPT_STRUCT(
        expression_ast::mofn_expr,
        (uint, m)
        (std::vector<expression_ast::expression>, terms)
)
BOOST_FUSION_ADAPT_STRUCT(
        expression_ast::expression,
        (expression_ast::type, expr)
)
BOOST_FUSION_ADAPT_STRUCT(
        expression_ast::rule_group,
        (std::string,                           group_id)
        (expression_ast::inputs_decl,           inputs)
        (expression_ast::outputs_decl,          outputs)
        (bool,                                  dense_flag)
        (std::vector<expression_ast::rule_def>, rules)
)


BOOST_FUSION_ADAPT_STRUCT(
        expression_ast::array,
        (std::string, name)
        (uint, begin)
        (uint, end)
)

#endif // FUSION_EXPRESSION_AST_HPP
