#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>

enum TokenType {
    PLUS,
    MINUS,
    STAR,
    SLASH,
    EXP,
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
};

struct Token {
    const TokenType type;
    const std::string lexeme;
    const size_t line;
    const size_t col;

    const double literal_double = 0;
    const bool literal_boolean = false;
    const std::string literal_string = "";

    Token(TokenType type_in, std::string lexeme_in, size_t line_in, size_t col_in);
    Token(TokenType type_in, std::string lexeme_in, size_t line_in, size_t col_in, double literal_double_in);
    Token(TokenType type_in, std::string lexeme_in, size_t line_in, size_t col_in, bool literal_boolean_in);
    Token(TokenType type_in, std::string lexeme_in, size_t line_in, size_t col_in, std::string literal_string_in);
};

#endif // TOKEN_H_
