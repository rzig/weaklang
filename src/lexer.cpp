#include "lexer.hpp"

void Lexer::lex(const std::string &to_lex) {
    size_t making_token_line = 0, making_token_col = 0; // For tracking position of tokens for better syntax error reporting
    size_t start = 0, current = 0; // For tracking interpretation of tokens
    std::vector<Token> tokens;
    while (current < to_lex.size()) {
        start = current;
        size_t next = current++;
        char start_token_c = to_lex.at(next);
        switch (start_token_c) {
            case '+': tokens.push_back(Token(PLUS, to_lex.substr(start, 1), making_token_line, making_token_col));break;
            case '-': tokens.push_back(Token(MINUS, to_lex.substr(start, 1), making_token_line, making_token_col));break;
            case '*': tokens.push_back(Token(STAR, to_lex.substr(start, 1), making_token_line, making_token_col));break;
            case '/': tokens.push_back(Token(SLASH, to_lex.substr(start, 1), making_token_line, making_token_col));break;
            case '^': tokens.push_back(Token(EXP, to_lex.substr(start, 1), making_token_line, making_token_col));break;
            case ',': tokens.push_back(Token(COMMA, to_lex.substr(start, 1), making_token_line, making_token_col));break;
            case '.': tokens.push_back(Token(DOT, to_lex.substr(start, 1), making_token_line, making_token_col));break;
            case ';': tokens.push_back(Token(SEMI, to_lex.substr(start, 1), making_token_line, making_token_col));break;
            case '#': while (current < to_lex.size() && to_lex.at(current++) != '\n');break;
            case '(': tokens.push_back(Token(LEFT_PAREN, to_lex.substr(start, 1), making_token_line, making_token_col));break;
            case ')': tokens.push_back(Token(RIGHT_PAREN, to_lex.substr(start, 1), making_token_line, making_token_col));break;
            case '{': tokens.push_back(Token(LEFT_BRACE, to_lex.substr(start, 1), making_token_line, making_token_col));break;
            case '}': tokens.push_back(Token(RIGHT_BRACE, to_lex.substr(start, 1), making_token_line, making_token_col));break;
            case '[': tokens.push_back(Token(LEFT_BRACK, to_lex.substr(start, 1), making_token_line, making_token_col));break;
            case ']': tokens.push_back(Token(RIGHT_BRACK, to_lex.substr(start, 1), making_token_line, making_token_col));break;
            case '!':
                if (current < to_lex.size() && to_lex.at(current) == '=') {
                    tokens.push_back(Token(EXCLA_EQUALS, to_lex.substr(start, 2), making_token_line, making_token_col));
                    current++;
                }
                else tokens.push_back(Token(EXCLA, to_lex.substr(start, 1), making_token_line, making_token_col));
                break;
            case '=':
                if (current < to_lex.size() && to_lex.at(current) == '=') {
                    tokens.push_back(Token(EQUALS_EQUALS, to_lex.substr(start, 2), making_token_line, making_token_col));
                    current++;
                }
                else tokens.push_back(Token(EQUALS, to_lex.substr(start, 1), making_token_line, making_token_col));
                break;
            case '>':
                if (current < to_lex.size() && to_lex.at(current) == '=') {
                    tokens.push_back(Token(GREATER_EQUALS, to_lex.substr(start, 2), making_token_line, making_token_col));
                    current++;
                }
                else tokens.push_back(Token(GREATER, to_lex.substr(start, 1), making_token_line, making_token_col));
                break;
            case '<':
                if (current < to_lex.size() && to_lex.at(current) == '=') {
                    tokens.push_back(Token(LESSER_EQUALS, to_lex.substr(start, 2), making_token_line, making_token_col));
                    current++;
                }
                else tokens.push_back(Token(LESSER, to_lex.substr(start, 1), making_token_line, making_token_col));
                break;
            case '"':
                current++;
                while(current < to_lex.size() && to_lex.at(current) != '\n' && to_lex.at(current) != '"') current++;
                if(current == to_lex.size() || to_lex.at(current) == '\n') {
                    had_error = true;
                }
                current++;
                tokens.push_back(Token(STRING, to_lex.substr(start+1, current-start-2), making_token_line, making_token_col));
                break;
            case '\n':
                making_token_col = -1;
                making_token_line++;
                break;
            // TODO add alphanumeric for identifier
            // ignore whitespace
            case ' ': case '\r': case '\t': break; 
            default:
                std::cout << start_token_c << std::endl; 
                had_error = true;
        }
        making_token_col++;
    }
    tokens.push_back(Token(END, "", making_token_line, making_token_col));
    std::cout << to_lex << std::endl;
    for (auto t : tokens) {
        std::cout << t.type << " " << t.lexeme << std::endl;
    }
    std::cout << (has_had_error() ? "true" : "false") << std::endl;
}

bool Lexer::has_had_error() {
    return had_error;
}
