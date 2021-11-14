#include "lexer.hpp"
#include "error.hpp"

/**
 * Builds a set containing the first character of each key in the provided map.
 */
std::unordered_set<char> build_prefix_set(std::unordered_map<std::string, TokenType> map) {
    std::unordered_set<char> set;
    for(auto p : map) {
        set.insert(p.first[0]);
    }
    return set;
}

/**
 * Returns the length of the longest key in the provided map.
 */ 
size_t get_longest_element(std::unordered_map<std::string, TokenType> map) {
    size_t max = 0;
    for(auto p : map) {
        max = std::max(p.first.size(), max);
    }
    return max;
}

//////////////////////////////////////////////////////////////////////////////
//                          LEXER CONFIGURATION                             //
//////////////////////////////////////////////////////////////////////////////
// Here, we configure our Lexer class with the tokens of our programming    //
// language, such as operators etc. As its name suggests, the keywords map  //
// contains all of our keywords. The operators_flow map contains operators  //
// and control flow statements. We build a couple utility items from the    //
// latter, including a set of all operator/flow prefixes, and the length of //
// the longest operator (so that our lexer can pick the most specific).     //
// Then, we define whitespace/newline/comment characters for the lexer.     //
// Note that we define A/O/T/F in keywords because they share keyword       //
// semantics in that they must be space-surrounded.                         //
//////////////////////////////////////////////////////////////////////////////

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

const std::unordered_map<std::string, TokenType> Lexer::operators_flow = {
    {"+", PLUS},
    {"-", MINUS},
    {"*", STAR},
    {"/", SLASH},
    {"^", EXP},
    {"@", AT},
    {",", COMMA},
    {";", SEMI},
    {"(", LEFT_PAREN},
    {")", RIGHT_PAREN},
    {"{", LEFT_BRACE},
    {"}", RIGHT_BRACE},
    {"[", LEFT_BRACK},
    {"]", RIGHT_BRACK},
    {"!", EXCLA},
    {"!=", EXCLA_EQUALS},
    {"=", EQUALS},
    {"==", EQUALS_EQUALS},
    {">", GREATER},
    {">=", GREATER_EQUALS},
    {"<", LESSER},
    {"<=", LESSER_EQUALS}
};

const std::unordered_set<char> Lexer::operator_flow_prefixes = build_prefix_set(Lexer::operators_flow);

const size_t Lexer::longest_operator_flow = get_longest_element(Lexer::operators_flow);

const std::unordered_set<char> Lexer::newlines = {'\n'};

const std::unordered_set<char> Lexer::whitespaces = {' ', '\t', '\r'};

const char Lexer::COMMENT_CHAR = '#';

const char Lexer::QUOTE_CHAR = '"';

//////////////////////////////////////////////////////////////////////////////
//                        LEXER IMPLEMENTATION                              //
//////////////////////////////////////////////////////////////////////////////

std::vector<Token> Lexer::lex(const std::string &to_lex) {
    size_t line = 0, column = 0; // For tracking position of tokens for better syntax error reporting
    size_t start_index = 0, current_index = 0; // For tracking interpretation of tokens

    std::vector<Token> tokens;

    while(current_index < to_lex.size()) {
        start_index = current_index;
        current_index++;
        char first_character = to_lex.at(start_index);

        TokenType token_type = EMPTY;

        if(starts_operator_flow(first_character)) {
            size_t operator_length = std::min(longest_operator_flow, to_lex.size()-current_index+1);
            while(not is_operator(to_lex.substr(start_index, operator_length))) operator_length--;
            current_index += operator_length - 1;
            token_type = operators_flow.at(to_lex.substr(start_index, operator_length));
        } else if (first_character == COMMENT_CHAR) {
            while(current_index < to_lex.size() && not is_newline(to_lex.at(current_index++)));
            line++;
            column = 0;
        } else if (first_character == QUOTE_CHAR) {
            current_index++; // move past the first quotation mark so we can
                             // capture the contents of the string
            while(current_index < to_lex.size() && not is_newline(to_lex.at(current_index)) && to_lex.at(current_index) != QUOTE_CHAR) current_index++;
            if(current_index == to_lex.size() or is_newline(to_lex.at(current_index))) {
                errors.push_back(Error("Unexpected string termination", line, column + current_index - start_index));
                line++;
                column = 0;
            }
            current_index++; // move past the last quotation mark
            token_type = STRING;
        } else if (is_newline(first_character)) {
            column = 0;
            line++;
        } else if (is_whitespace(first_character)) {
            // Ignore the character
        } else if (is_digit(first_character)) {
            while(current_index < to_lex.size() && is_digit(to_lex.at(current_index))) current_index++;
            if(current_index == to_lex.size()) {
                // Number terminated the file, don't check for a . after it
            } else if (to_lex.at(current_index) == '.') {
                current_index++;
                while(current_index < to_lex.size() && is_digit(to_lex.at(current_index))) current_index++;
            }
            token_type = NUMBER;
        } else if (is_alpha(first_character)) {
            while(current_index < to_lex.size() && is_alpha(to_lex.at(current_index)) && (not is_whitespace(to_lex.at(current_index)))) current_index++;
            std::string lexeme_check = to_lex.substr(start_index, current_index-start_index);
            if(is_keyword(lexeme_check)) {
                token_type = keywords.at(lexeme_check);
            } else {
                token_type = IDENTIFIER;
            }
        } else {
            errors.push_back(Error("Unrecognized character", line, column));
        }

        if(token_type != EMPTY) {
            std::string lexeme = to_lex.substr(start_index, current_index);
            tokens.push_back(Token(token_type, lexeme, line, column));
        }

        column += current_index - start_index;
    }

    tokens.push_back(Token(END, "", line, column));
    return tokens;
}

std::vector<Error> Lexer::get_errors() {
    return errors;
}

//////////////////////////////////////////////////////////////////////////////
//                        LEXER UTILITY FUNCTIONS                           //
//////////////////////////////////////////////////////////////////////////////

bool Lexer::is_operator(std::string str) {
    return operators_flow.find(str) != operators_flow.end();
}

bool Lexer::has_had_error() {
    return errors.size() > 0;
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

bool Lexer::starts_operator_flow(char c) {
    return operator_flow_prefixes.find(c) != operator_flow_prefixes.end();
}

bool Lexer::is_whitespace(char c) {
    return whitespaces.find(c) != whitespaces.end();
}

bool Lexer::is_newline(char c) {
    return newlines.find(c) != newlines.end();
}