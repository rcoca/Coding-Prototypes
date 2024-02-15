#include <boost/variant/apply_visitor.hpp>
#include "expression_ast_print.hpp"

expression_ast_print::expression_ast_print(std::ostream& out_):out(out_)
{
}

void expression_ast_print::operator()(expression_ast::nil const&) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    out<<"(nil)"<<std::endl;
}

void expression_ast_print::operator()(expression_ast::program_expr const & expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    (*this)(expr.classes);
    for(auto const& rule_grp:expr.rule_groups)
        (*this)(rule_grp);

    for(std::size_t i=0;i<expr.rules_instances.size();++i)
    {
        (*this)(expr.rules_instances[i]);
        if(i<expr.rules_instances.size()-1)out<<", ";
    }
    out<<';'<<std::endl;

}
void expression_ast_print::operator()(expression_ast::rule_def const & expr) const
{        
    //out<<__PRETTY_FUNCTION__<<std::endl;
    out<<'\t'<<expr.target<<" :- ";
    for(std::size_t i=0;i<expr.precedents.size();++i)
    {
        (*this)(expr.precedents[i]);
        if(i<expr.precedents.size()-1)out<<", ";
    }
    out<<';'<<std::endl;
}
void expression_ast_print::operator()(expression_ast::inputs_decl const & expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    out<<"\tinputs : ";
    for(std::size_t i=0;i<expr.identifiers.size();++i)
    {
        (*this)(expr.identifiers[i]);
        if(i<expr.identifiers.size()-1)out<<", ";
    }
    out<<';'<<std::endl;
}

void expression_ast_print::operator()(expression_ast::outputs_decl const & expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    out<<"\toutputs : ";
    for(std::size_t i=0;i<expr.identifiers.size();++i)
    {
        (*this)(expr.identifiers[i]);
        if(i<expr.identifiers.size()-1)out<<", ";
    }
    out<<';'<<std::endl;
}

void expression_ast_print::operator()(expression_ast::rule_group const & expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    out<<"rules "<<expr.group_id<<std::endl;
    out<<'{'<<std::endl;
    (*this)(expr.inputs);
    (*this)(expr.outputs);
    if(expr.dense_flag) out<<"\tdense: true;";
    else out<<"\tdense: false;";
    out<<std::endl;
    for(auto const& rule:expr.rules)
        (*this)(rule);

    out<<'}'<<std::endl;
}

void expression_ast_print::operator()(expression_ast::classes_decl const & expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    out<<"classes"<<std::endl;
    out<<"{"<<std::endl;
    for(auto const& c : expr.declarations)   (*this)(c);
    out<<"}"<<std::endl;
}

void expression_ast_print::operator()(expression_ast::class_decl const& expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    out<<'\t'<<expr.name<<" : ";
    for(std::size_t i=0;i<expr.enums.size();++i)
    {
        out<<expr.enums[i];
        if(i<expr.enums.size()-1)out<<", ";
    }
    out<<';'<<std::endl;
}


void expression_ast_print::operator()(expression_ast::and_expr const& expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    out<<"and (";
    for(std::size_t i=0;i<expr.terms.size();++i)
    {
        (*this)(expr.terms[i]);
        if(i<expr.terms.size()-1)out<<", ";
    }
    out<<")";
}
void expression_ast_print::operator()(expression_ast::seq_expr const& expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    out<<"seq (";
    for(std::size_t i=0;i<expr.terms.size();++i)
    {
        (*this)(expr.terms[i]);
        if(i<expr.terms.size()-1)out<<", ";
    }
    out<<")";
}

void expression_ast_print::operator()(std::string const &expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    out<<expr;
}

void expression_ast_print::operator()(expression_ast::array const &expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    out<<expr.name<<'['<<expr.begin<<".."<<expr.end<<']';
}

void expression_ast_print::operator()(expression_ast::identifier const& expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    boost::apply_visitor(*this,expr.expr);
}
void expression_ast_print::operator()(expression_ast::or_expr const& expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    out<<"or (";
    for(std::size_t i=0;i<expr.terms.size();++i)
    {
        (*this)(expr.terms[i]);
        if(i<expr.terms.size()-1)out<<", ";
    }
    out<<")";
}

void expression_ast_print::operator()(expression_ast::mofn_expr const& expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    out<<expr.m<<" of {";
    for(std::size_t i=0;i<expr.terms.size();++i)
    {
        (*this)(expr.terms[i]);
        if(i<expr.terms.size()-1)out<<", ";
    }
    out<<"}";
}

void expression_ast_print::operator()(expression_ast::not_expr const& expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    out<<"~ ";
    boost::apply_visitor(*this,expr.term.expr);
    out<<" ";
}

void expression_ast_print::operator()(expression_ast::expression const& expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    boost::apply_visitor(*this,expr.expr);
}

void expression_ast_print::operator()(expression_ast::type const& expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    boost::apply_visitor(*this,expr);
}

void expression_ast_print::operator()(expression_ast const& expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    boost::apply_visitor(*this,expr.generic_expression);
}

void expression_ast_print::operator()(expression_ast::func_call const& expr) const
{
    //out<<__PRETTY_FUNCTION__<<std::endl;
    out<<expr.func<<" (";
    (*this)(expr.arg);
    out<<")";
}
/*********************************************************************/
/**********************overloads for boost__spirit_debug_node*********/
/*********************************************************************/

std::ostream& operator<<(std::ostream& os,expression_ast::type const& t)
{
    expression_ast_print p(os);
    boost::apply_visitor(p,t);
    return os;
}
std::ostream& operator<<(std::ostream& os,expression_ast const& t)
{
    expression_ast_print p(os);
    boost::apply_visitor(p,t.generic_expression);
    return os;
}

std::ostream& operator<<(std::ostream& os,expression_ast::class_decl const& t)
{
    expression_ast_print p(os);
    p(t);
    return os;
}
std::ostream& operator<<(std::ostream& os,expression_ast::classes_decl const& t)
{
    expression_ast_print p(os);
    p(t);
    return os;
}
