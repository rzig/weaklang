#ifndef LEXER_H_
#define LEXER_H_

#include <iostream>
#include <vector>
#include <unordered_map>

#include "token.hpp"

class Lexer {

public:
    std::vector<Token> lex(const std::string &to_lex);
    bool has_had_error();
private:
    bool is_digit(char c);
    bool is_alpha(char c);
    bool is_keyword(std::string str);
    bool had_error = false;
    static const std::unordered_map<std::string, TokenType> keywords;
};

#endif // LEXER_H_
