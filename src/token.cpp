#include "token.hpp"

Token::Token(TokenType type_in, std::string lexeme_in, size_t line_in, size_t col_in): type(type_in), lexeme(lexeme_in), line(line_in), col(col_in) {}

Token::Token(TokenType type_in, std::string lexeme_in, size_t line_in, size_t col_in, double literal_double_in): type(type_in), lexeme(lexeme_in), line(line_in), col(col_in), literal_double(literal_double_in) {}

Token::Token(TokenType type_in, std::string lexeme_in, size_t line_in, size_t col_in, bool literal_boolean_in): type(type_in), lexeme(lexeme_in), line(line_in), col(col_in), literal_boolean(literal_boolean_in) {}

Token::Token(TokenType type_in, std::string lexeme_in, size_t line_in, size_t col_in, std::string literal_string_in): type(type_in), lexeme(lexeme_in), line(line_in), col(col_in), literal_string(literal_string_in) {}
