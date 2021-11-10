#ifndef PARSER_H_
#define PARSER_H_

#include "token.hpp"

#include <vector>

class Parser {
    public:
    Parser(std::vector<Token> input); 
    private:
    std::vector<Token> tokens; 
};

#endif // PARSER_H_