// This file is part of weak-lang.
// weak-lang is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
// weak-lang is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
// You should have received a copy of the GNU Affero General Public License
// along with weak-lang. If not, see <https://www.gnu.org/licenses/>.

#ifndef LEXER_H_
#define LEXER_H_

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "token.hpp"
#include "error.hpp"

class Lexer {

public:
    std::vector<Token> lex(const std::string &to_lex);
    bool has_had_error();
    std::vector<Error> get_errors();
    std::string print_errors(); 
private:
    std::vector<Error> errors;
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
