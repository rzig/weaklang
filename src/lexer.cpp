#include "lexer.hpp"

void Lexer::lex(const std::string &to_lex) {
    size_t making_token_line = 0, making_token_col = 0; // For tracking position of tokens for better syntax error reporting
    size_t start = 0, current = 0; // For tracking interpretation of tokens
    std::vector<Token> tokens;
    while (current < to_lex.size()) {
        start = current;
        char start_token_c = to_lex.at(current++);
        switch (start_token_c) {
            case '+': tokens.push_back(Token(PLUS, to_lex.substr(start, current), making_token_line, making_token_col));break;
            case '-': tokens.push_back(Token(MINUS, to_lex.substr(start, current), making_token_line, making_token_col));break;
            case '*': tokens.push_back(Token(STAR, to_lex.substr(start, current), making_token_line, making_token_col));break;
            case '/': tokens.push_back(Token(SLASH, to_lex.substr(start, current), making_token_line, making_token_col));break;
            case ',': tokens.push_back(Token(COMMA, to_lex.substr(start, current), making_token_line, making_token_col));break;
            case '.': tokens.push_back(Token(DOT, to_lex.substr(start, current), making_token_line, making_token_col));break;
            case ';': tokens.push_back(Token(SEMI, to_lex.substr(start, current), making_token_line, making_token_col));break;
            case '(': tokens.push_back(Token(LEFT_PAREN, to_lex.substr(start, current), making_token_line, making_token_col));break;
            case ')': tokens.push_back(Token(RIGHT_PAREN, to_lex.substr(start, current), making_token_line, making_token_col));break;
            case '{': tokens.push_back(Token(LEFT_BRACE, to_lex.substr(start, current), making_token_line, making_token_col));break;
            case '}': tokens.push_back(Token(RIGHT_BRACE, to_lex.substr(start, current), making_token_line, making_token_col));break;
            case '[': tokens.push_back(Token(LEFT_BRACK, to_lex.substr(start, current), making_token_line, making_token_col));break;
            case ']': tokens.push_back(Token(RIGHT_BRACK, to_lex.substr(start, current), making_token_line, making_token_col));break;
            case '!': tokens.push_back(Token(EXCLA, to_lex.substr(start, current), making_token_line, making_token_col));break;
            case '=': tokens.push_back(Token(EQUALS, to_lex.substr(start, current), making_token_line, making_token_col));break;
            case '>': tokens.push_back(Token(GREATER, to_lex.substr(start, current), making_token_line, making_token_col));break;
            case '<': tokens.push_back(Token(LESSER, to_lex.substr(start, current), making_token_line, making_token_col));break;
        }
    }
}
