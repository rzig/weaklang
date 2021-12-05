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

#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>

enum TokenType {
    PLUS,
    MINUS,
    STAR,
    SLASH,
    EXP,
    AT,
    COMMA,
    SEMI,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_BRACK,
    RIGHT_BRACK,
    EXCLA,
    EQUALS,
    GREATER,
    LESSER,
    EXCLA_EQUALS,
    EQUALS_EQUALS,
    GREATER_EQUALS,
    LESSER_EQUALS,
    AND,
    OR,
    TRUE,
    FALSE,
    FUNCTION,
    OPERATOR,
    NIL,
    IF,
    PRINT,
    RETURN,
    LET,
    WHILE,
    END,
    IDENTIFIER,
    NUMBER,
    BOOLEAN,
    STRING,
    SHAPE,
    AS_SHAPE,
    EMPTY,
    ASSERT
};

struct Token {
    const TokenType type;
    const std::string lexeme;
    const size_t line;
    const size_t col;

    const double literal_double = 0;
    const std::string literal_string = "";

    Token(TokenType type_in, std::string lexeme_in, size_t line_in, size_t col_in);
};

std::string print_token_type(TokenType type);

#endif // TOKEN_H_
