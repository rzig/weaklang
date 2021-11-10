#ifndef PARSER_H_
#define PARSER_H_

#include "token.hpp"
#include "expr.hpp"

#include <vector>

class Parser {
public:
    Parser(std::vector<Token> input); 
private:
    std::vector<Token> tokens; 

    Expr expression(); 
    Expr equality(); 
};

#endif // PARSER_H_