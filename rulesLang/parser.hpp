#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <boost/optional.hpp>
#include "expression_ast.hpp"

boost::optional<expression_ast> parse_rules(std::string s);

#endif // PARSER_HPP
