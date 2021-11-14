
//////////////////////////////////////////////////////////////////////////////
//                             Framework Set-up                             //
//////////////////////////////////////////////////////////////////////////////
#include "catch.hpp"

//////////////////////////////////////////////////////////////////////////////
//                                 Includes                                 //
//////////////////////////////////////////////////////////////////////////////
#include "lexer.hpp"
#include<iostream>
#include<fstream>
#include<sstream>

//////////////////////////////////////////////////////////////////////////////
//                                Test Cases                                //
//////////////////////////////////////////////////////////////////////////////

void no_error(std::string str){
    Lexer lex; 
    lex.lex(str); 
    REQUIRE(!lex.has_had_error());
}

void error(std::string str) {
    Lexer lex;
    lex.lex(str);
    REQUIRE(lex.has_had_error());
}

std::vector<Token> getTokens(std::string str) {
    Lexer lex;
    return lex.lex(str);
}

void only_returns_token(std::string str, TokenType type) {
    std::vector<Token> tokens = getTokens(str);
    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0].type == type);
    REQUIRE(tokens[1].type == END);
}

using TokenTypeList = std::initializer_list<TokenType>;

void expect_tokens(std::string str, TokenTypeList types_expected) {
    std::vector<Token> tokens_received = getTokens(str);
    REQUIRE(tokens_received.size() == types_expected.size());
    auto it = types_expected.begin();
    for(size_t i = 0; i < std::min(types_expected.size(), tokens_received.size()); i++) {
        REQUIRE(tokens_received[i].type == *it);
        it++;
    }
}

TEST_CASE("Simplistic tokens", "[lexer]"){
    SECTION("Simple math"){
        no_error("+");
        only_returns_token("+", PLUS);
        no_error("-");
        only_returns_token("-", MINUS);
        no_error("*"); 
        only_returns_token("*", STAR);;
        no_error("/"); 
        only_returns_token("/", SLASH);
        no_error("^"); 
        only_returns_token("^", EXP);
        no_error("@");
        only_returns_token("@", AT);
    }
    SECTION("Punctuation"){
        no_error(","); 
        only_returns_token(",", COMMA);
        no_error(";"); 
        only_returns_token(";", SEMI);
    }
    SECTION("Comments"){
        no_error("#"); 
        REQUIRE(getTokens("#").size() == 1);
        REQUIRE(getTokens("#")[0].type == END);
    }
    SECTION("Parentheses"){
        no_error("("); 
        only_returns_token("(", LEFT_PAREN);
        no_error(")"); 
        only_returns_token(")", RIGHT_PAREN);
    }
    SECTION("Block"){
        no_error("{"); 
        only_returns_token("{", LEFT_BRACE);
        no_error("}"); 
        only_returns_token("}", RIGHT_BRACE);
    }
    SECTION("Brackets"){
        no_error("["); 
        only_returns_token("[", LEFT_BRACK);
        no_error("]"); 
        only_returns_token("]", RIGHT_BRACK);
    }
    SECTION("Assignment"){
        no_error("=");
        only_returns_token("=", EQUALS);
    }
    SECTION("Boolean"){
        no_error("!"); 
        only_returns_token("!", EXCLA);
        no_error("=="); 
        only_returns_token("==", EQUALS_EQUALS);
        no_error(">");
        only_returns_token(">", GREATER);
        no_error("<");
        only_returns_token("<", LESSER);
        no_error(">=");
        only_returns_token(">=", GREATER_EQUALS);
        no_error("<=");
        only_returns_token("<=", LESSER_EQUALS);
    }
    SECTION("String literals") {
        no_error("\"Hello world\"");
        only_returns_token("\"Hello world\"", STRING);
    }
}

TEST_CASE("Single-line strings", "[lexer]") {
    SECTION("ignores whitespace only tabs") {
        no_error("()\t{}#");
        expect_tokens("()\t{}#", {LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, END});
    }
    SECTION("ignores whitespace only spaces") {
        no_error("()  {} #");
        expect_tokens("()  {} #", {LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, END});
    }
    SECTION("ignores whitespace only returns") {
        no_error("()\r{}#");
        expect_tokens("()\r{}#", {LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, END});
    }
    SECTION("ignores whitespace combination") {
        no_error("()\r{\t}  #");
        expect_tokens("()\r{\t}  #", {LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, END});
    }

    SECTION("Ignores everything after comments") {
        no_error("()#[\"hello world");
        expect_tokens("()#[\"hello world", {LEFT_PAREN, RIGHT_PAREN, END});
    }
}

TEST_CASE("Identifiers", "[lexer]") {
    SECTION("Accepts alpha characters") {
        no_error("abc;");
        std::vector<Token> tokens = getTokens("abc;");
        expect_tokens("abc;", {IDENTIFIER, SEMI, END});
        REQUIRE(getTokens("abc;")[0].lexeme == "abc");
    }

    SECTION("Accepts alpha followed by alphanumeric and underscore") {
        no_error("abc_def = 1");
        expect_tokens("abc_def = 1", {IDENTIFIER, EQUALS, NUMBER, END});
        REQUIRE(getTokens("abc_def = 1")[0].lexeme == "abc_def");
    }
}

TEST_CASE("Keywords", "[lexer]") {
    // It would be cumbersome to create test cases for each
    // boolean-related operator, so we group them in a way that
    // resembles real inputs to the lexer.
    SECTION("True, Or, False") {
        no_error("T O F");
        expect_tokens("T O F", {TRUE, OR, FALSE, END});
    }
    SECTION("Nil, And, True") {
        no_error("N A T");
        expect_tokens("N A T", {NIL, AND, TRUE, END});
    }

    SECTION("function") {
        no_error("f m");
        expect_tokens("f m", {FUNCTION, IDENTIFIER, END});
    }

    SECTION("operator") {
        no_error("o app");
        expect_tokens("o app", {OPERATOR, IDENTIFIER, END});
    }

    SECTION("If") {
        no_error("i (c==b)");
        expect_tokens("i (c==b)", {IF, LEFT_PAREN, IDENTIFIER, EQUALS_EQUALS, IDENTIFIER, RIGHT_PAREN, END});
    }

    SECTION("Return") {
        no_error("r mat");
        expect_tokens("r mat", {RETURN, IDENTIFIER, END});
    }

    SECTION("While") {
        no_error("w (c==b)");
        expect_tokens("w (c==b)", {WHILE, LEFT_PAREN, IDENTIFIER, EQUALS_EQUALS, IDENTIFIER, RIGHT_PAREN, END});
    }

    SECTION("Shape") {
        no_error("s x");
        expect_tokens("s x", {SHAPE, IDENTIFIER, END});
    }

    SECTION("As Shape") {
        no_error("[] sa [2,2]");
        expect_tokens("[] sa [2,2]", {LEFT_BRACK, RIGHT_BRACK, AS_SHAPE, LEFT_BRACK, NUMBER, COMMA, NUMBER, RIGHT_BRACK, END});
    }

    SECTION("Let") {
        no_error("a x = 3");
        expect_tokens("a x = 3", {LET, IDENTIFIER, EQUALS, NUMBER, END});
    }

    SECTION("prioritizes identifiers") {
        // here we implement a few one off tests to make sure
        // that if an identifier starts with the same letter or letters
        // as an operator that we will still keep it as an identifier

        // first we try something overlapping with a one letter operator (a)
        no_error("a app");
        expect_tokens("a app", {LET, IDENTIFIER, END});
        // next we try something overlapping with a two-letter
        // operator (sa)
        no_error("a sap");
        expect_tokens("a sap", {LET, IDENTIFIER, END});
    }
}

TEST_CASE("Complex input", "[lexer]") {
    std::ifstream file_in("./tests/test.weak");
    std::stringstream stream;
    stream << file_in.rdbuf();
    file_in.close();
    std::string file_str = stream.str();
    no_error(file_str);
    expect_tokens(file_str, {OPERATOR, IDENTIFIER, LEFT_PAREN, IDENTIFIER, COMMA, IDENTIFIER, RIGHT_PAREN, LEFT_BRACE, RETURN, IDENTIFIER, EXP, LEFT_PAREN, IDENTIFIER, STAR, NUMBER, RIGHT_PAREN, SEMI, RIGHT_BRACE, OPERATOR, IDENTIFIER, LEFT_PAREN, IDENTIFIER, COMMA, IDENTIFIER, RIGHT_PAREN, LEFT_BRACE, RETURN, LEFT_PAREN, SHAPE, IDENTIFIER, RIGHT_PAREN, PLUS, LEFT_PAREN, SHAPE, IDENTIFIER, RIGHT_PAREN, SEMI, RIGHT_BRACE, FUNCTION, IDENTIFIER, LEFT_PAREN, IDENTIFIER, COMMA, IDENTIFIER, COMMA, IDENTIFIER, RIGHT_PAREN, LEFT_BRACE, LET, IF, EQUALS, NUMBER, SEMI, LET, IDENTIFIER, EQUALS, LEFT_BRACK, IDENTIFIER, COMMA, IDENTIFIER, COMMA, IDENTIFIER, COMMA, IDENTIFIER, RIGHT_BRACK, AS_SHAPE, LEFT_BRACK, IDENTIFIER, COMMA, IDENTIFIER, RIGHT_BRACK, SEMI, WHILE, LEFT_PAREN, IF, LESSER, IDENTIFIER, RIGHT_PAREN, LEFT_BRACE, IDENTIFIER, EQUALS, IDENTIFIER, IDENTIFIER, IDENTIFIER, SEMI, IF, EQUALS, IF, PLUS, NUMBER, SEMI, RIGHT_BRACE, IF, LEFT_PAREN, IDENTIFIER, LEFT_BRACK, NUMBER, RIGHT_BRACK, LEFT_BRACK, NUMBER, RIGHT_BRACK, EXCLA_EQUALS, NUMBER, RIGHT_PAREN, LEFT_BRACE, RETURN, TRUE, SEMI, RIGHT_BRACE, RETURN, FALSE, SEMI, RIGHT_BRACE, PRINT, FUNCTION, LEFT_PAREN, NUMBER, COMMA, NUMBER, COMMA, NUMBER, RIGHT_PAREN, SEMI, PRINT, STRING, SEMI, PRINT, NIL, SEMI, PRINT, FUNCTION, LEFT_PAREN, NUMBER, COMMA, NUMBER, COMMA, NUMBER, RIGHT_PAREN, OR, FUNCTION, LEFT_PAREN, NUMBER, COMMA, NUMBER, COMMA, NUMBER, RIGHT_PAREN, SEMI, PRINT, NUMBER, GREATER, NUMBER, SEMI, PRINT, NUMBER, GREATER_EQUALS, NUMBER, SEMI, PRINT, NUMBER, LESSER, NUMBER, SEMI, PRINT, NUMBER, LESSER_EQUALS, NUMBER, SEMI, PRINT, NUMBER, PLUS, NUMBER, MINUS, NUMBER, STAR, NUMBER, SLASH, NUMBER, SEMI, PRINT, LEFT_BRACK, RIGHT_BRACK, AT, LEFT_BRACK, RIGHT_BRACK, SEMI, END});
    // Now we'll spot-check a few tokens for correct row/column data
    // (doing so for all nearly 200 would be excessive)
    std::vector<Token> tokens = getTokens(file_str);
    REQUIRE(tokens[50].line == 10);
    REQUIRE(tokens[50].col == 7);
    REQUIRE(tokens[100].line == 16);
    REQUIRE(tokens[100].col == 14);
    REQUIRE(tokens[150].line == 26);
    REQUIRE(tokens[150].col == 5);
}

TEST_CASE("Errors", "[lexer]") {
    // It's kind of hard to add non-ascii symbols, so no test
    // for that for the time being.
    SECTION("string end of file") {
        Lexer lex;
        lex.lex("\"hello w");
        REQUIRE(lex.has_had_error());
        REQUIRE(lex.get_errors().size() == 1);
        REQUIRE(lex.get_errors()[0].message == "Unexpected string termination");
        REQUIRE(lex.get_errors()[0].line == 0);
        REQUIRE(lex.get_errors()[0].column == 8);
    }

    SECTION("string newline") {
        Lexer lex;
        lex.lex("\"hello w\n");
        REQUIRE(lex.has_had_error());
        REQUIRE(lex.get_errors().size() == 1);
        REQUIRE(lex.get_errors()[0].message == "Unexpected string termination");
        REQUIRE(lex.get_errors()[0].line == 0);
        REQUIRE(lex.get_errors()[0].column == 8);
    }
}