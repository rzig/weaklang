#ifndef LEXER_H_
#define LEXER_H_

#include <iostream>
#include <vector>

#include "token.hpp"

class Lexer {

public:
    std::vector<Token> lex(const std::string &to_lex);
    bool has_had_error();
private:
    bool is_digit(char c);
    bool is_alpha(char c);
    bool is_alpha_digit(char c);
    bool had_error = false;
};

#endif // LEXER_H_
