#ifndef LEXER_H_
#define LEXER_H_

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "token.hpp"

class Lexer {

public:
    std::vector<Token> lex(const std::string &to_lex);
    std::vector<Token> lex2(const std::string &to_lex);
    bool has_had_error();
private:
    bool had_error = false;
    bool is_digit(char c);
    bool is_alpha(char c);
    bool is_keyword(std::string str);
    bool is_newline(char c);
    bool is_whitespace(char c);
    bool starts_operator_flow(char c);
    bool is_operator(std::string str);
    static const std::unordered_map<std::string, TokenType> keywords;
    static const std::unordered_map<std::string, TokenType> operators_flow;
    static const std::unordered_set<char> operator_flow_prefixes;
    static const std::unordered_set<char> newlines;
    static const std::unordered_set<char> whitespaces;
    static const char COMMENT_CHAR;
    static const char QUOTE_CHAR;
    static const size_t longest_operator_flow;
};

#endif // LEXER_H_
