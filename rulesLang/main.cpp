#include <iostream>
#include <sstream>
#include <fstream>
#include "parser.hpp"
#include "expression_ast_print.cpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "usage: " << argv[0] << ' ' << "rules_file_name" << std::endl;
        return 1;
    }
    std::ifstream rulesFile(argv[1]);
    if (!rulesFile.good())
    {
        std::cerr << "could not open " << argv[1] << std::endl;
        return 1;
    }
    std::stringstream buffer;
    buffer << rulesFile.rdbuf();
    auto rulesAST = parse_rules(buffer.str());
    if (rulesAST.has_value())
    {
        expression_ast_print printer(std::cout);
        printer(rulesAST.value());
    }
    else
    {
        std::cerr << "parse error" << std::endl;
        return 1;
    }

    return 0;
}