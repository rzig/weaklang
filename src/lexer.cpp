#include "lexer.hpp"

const std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"A", AND},
    {"O", OR},
    {"T", TRUE},
    {"F", FALSE},
    {"f", FUNCTION},
    {"o", OPERATOR},
    {"N", NIL},
    {"i", IF},
    {"p", PRINT},
    {"r", RETURN},
    {"a", LET},
    {"w", WHILE},
    {"s", SHAPE},
    {"sa", AS_SHAPE}
};

std::vector<Token> Lexer::lex(const std::string &to_lex) {
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
            case '@': tokens.push_back(Token(AT, to_lex.substr(start, 1), making_token_line, making_token_col));break;
            case ',': tokens.push_back(Token(COMMA, to_lex.substr(start, 1), making_token_line, making_token_col));break;
            case ';': tokens.push_back(Token(SEMI, to_lex.substr(start, 1), making_token_line, making_token_col));break;
            case '#': {
                while (current < to_lex.size() && to_lex.at(current++) != '\n');
                making_token_line++;
                making_token_col = start - current;
                break;
            }
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
                    making_token_line++;
                    making_token_col = 0;
                    had_error = true;
                }
                current++;
                tokens.push_back(Token(STRING, to_lex.substr(start + 1, current - start - 2), making_token_line, making_token_col, to_lex.substr(start + 1, current - start - 2)));
                break;
            case '\n':
                making_token_col = start - current;
                making_token_line++;
                break;
            // TODO add alphanumeric for identifier
            // ignore whitespace
            case ' ': case '\r': case '\t': break; 
            default:
                if (is_digit(start_token_c)) {
                    while (current < to_lex.size() && is_digit(to_lex.at(current))) current++;
                    if (current == to_lex.size()) {
                        tokens.push_back(Token(NUMBER, to_lex.substr(start, current - start), making_token_line, making_token_col, stod(to_lex.substr(start, current - start))));
                        break;
                    }
                    if (to_lex.at(current) == '.') {
                        current++;
                        while (current < to_lex.size() && is_digit(to_lex.at(current))) current++;
                    }
                    tokens.push_back(Token(NUMBER, to_lex.substr(start, current - start), making_token_line, making_token_col, stod(to_lex.substr(start, current - start))));
                }
                else if (is_alpha(start_token_c)) {
                    while(current < to_lex.size() && (is_alpha(to_lex.at(current)) || is_digit(to_lex.at(current)))) current++;
                    std::string lexeme_check = to_lex.substr(start, current - start);
                    if(is_keyword(lexeme_check)) {
                        tokens.push_back(Token(keywords.at(lexeme_check), lexeme_check, making_token_line, making_token_col));
                    } else {
                        tokens.push_back(Token(IDENTIFIER, lexeme_check, making_token_line, making_token_col));
                    }
                }
                else had_error = true;
        }
        making_token_col += current - start;
    }
    tokens.push_back(Token(END, "", making_token_line, making_token_col));
    std::cout << to_lex << std::endl;
    for (auto t : tokens) {
        std::cout << print_token_type(t.type) << " ";
    }
    std::cout << std::endl;
    std::cout << (has_had_error() ? "true" : "false") << std::endl;
    return tokens;
}

bool Lexer::has_had_error() {
    return had_error;
}

bool Lexer::is_digit(char c) {
    return c <= '9' && c >= '0';
}

bool Lexer::is_alpha(char c) {
    return c == '_' || (c <= 'Z' && c >= 'A') || (c <= 'z' && c >= 'a');
}

bool Lexer::is_keyword(std::string str) {
    return keywords.find(str) != keywords.end();
}