#ifndef EXPRESSION_AST_PRINT_HPP
#define EXPRESSION_AST_PRINT_HPP
#include <iostream>
#include "expression_ast.hpp"

class expression_ast_print
{
    std::ostream &out;
public:
    typedef void result_type;

    expression_ast_print(std::ostream& out_);

    void operator()(expression_ast::nil const&) const ;

    void operator()(expression_ast::program_expr const &expr) const ;

    void operator()(expression_ast::rule_group const &expr) const ;

    void operator()(expression_ast::rule_def const &expr) const ;

    void operator()(expression_ast::inputs_decl const &expr) const ;

    void operator()(expression_ast::outputs_decl const &expr) const ;

    void operator()(expression_ast::identifier const &expr) const ;

    void operator()(std::string const &expr) const ;

    void operator()(expression_ast::array const &expr) const ;


    void operator()(expression_ast::classes_decl const &expr) const ;

    void operator()(expression_ast::class_decl const &expr) const ;

    void operator()(expression_ast::and_expr const &expr) const ;

    void operator()(expression_ast::seq_expr const &expr) const ;

    void operator()(expression_ast::or_expr const &expr) const ;

    void operator()(expression_ast::mofn_expr const &expr) const ;

    void operator()(expression_ast::not_expr const &expr) const ;

    void operator()(expression_ast::expression const &expr) const ;

    void operator()(expression_ast::func_call const &expr) const ;

     void operator()(expression_ast::type const &expr) const ;

    void operator()(expression_ast const &expr) const ;
};


std::ostream& operator<<(std::ostream& os,expression_ast::type const& t);
std::ostream& operator<<(std::ostream& os,expression_ast const& t);
std::ostream& operator<<(std::ostream& os,expression_ast::class_decl const& t);
std::ostream& operator<<(std::ostream& os,expression_ast::classes_decl const& t);

#endif // EXPRESSION_AST_PRINT_HPP
