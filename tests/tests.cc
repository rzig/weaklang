
//////////////////////////////////////////////////////////////////////////////
//                             Framework Set-up                             //
//////////////////////////////////////////////////////////////////////////////
#include "catch.hpp"

//////////////////////////////////////////////////////////////////////////////
//                                 Includes                                 //
//////////////////////////////////////////////////////////////////////////////
#include "lexer.hpp"

//////////////////////////////////////////////////////////////////////////////
//                                Test Cases                                //
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////

void no_error(std::string str){
    Lexer lex; 
    lex.lex(str); 
    REQUIRE(!lex.has_had_error());
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
    }
    SECTION("Punctuation"){
      no_error(","); 
      only_returns_token(",", COMMA);
      no_error("."); 
      only_returns_token(".", DOT);
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
      std::vector<Token> tokens = getTokens("()\t{}#");
      REQUIRE(tokens.size() == 5);
      REQUIRE(tokens[0].type == LEFT_PAREN);
      REQUIRE(tokens[1].type == RIGHT_PAREN);
      REQUIRE(tokens[2].type == LEFT_BRACE);
      REQUIRE(tokens[3].type == RIGHT_BRACE);
      REQUIRE(tokens[4].type == END);
    }
    SECTION("ignores whitespace only spaces") {
      no_error("()  {} #");
      std::vector<Token> tokens = getTokens("()  {} #");
      REQUIRE(tokens.size() == 5);
      REQUIRE(tokens[0].type == LEFT_PAREN);
      REQUIRE(tokens[1].type == RIGHT_PAREN);
      REQUIRE(tokens[2].type == LEFT_BRACE);
      REQUIRE(tokens[3].type == RIGHT_BRACE);
      REQUIRE(tokens[4].type == END);
    }
    SECTION("ignores whitespace only returns") {
      no_error("()\r{}#");
      std::vector<Token> tokens = getTokens("()\r{}#");
      REQUIRE(tokens.size() == 5);
      REQUIRE(tokens[0].type == LEFT_PAREN);
      REQUIRE(tokens[1].type == RIGHT_PAREN);
      REQUIRE(tokens[2].type == LEFT_BRACE);
      REQUIRE(tokens[3].type == RIGHT_BRACE);
      REQUIRE(tokens[4].type == END);
    }
    SECTION("ignores whitespace combination") {
      no_error("()\r{\t}  #");
      std::vector<Token> tokens = getTokens("()\r{\t}  #");
      REQUIRE(tokens.size() == 5);
      REQUIRE(tokens[0].type == LEFT_PAREN);
      REQUIRE(tokens[1].type == RIGHT_PAREN);
      REQUIRE(tokens[2].type == LEFT_BRACE);
      REQUIRE(tokens[3].type == RIGHT_BRACE);
      REQUIRE(tokens[4].type == END);
    }

    SECTION("Ignores everything after comments") {
      no_error("()#[\"hello world");
      std::vector<Token> tokens = getTokens("()#[\"hello world");
      REQUIRE(tokens.size() == 3);
      REQUIRE(tokens[0].type == LEFT_PAREN);
      REQUIRE(tokens[1].type == RIGHT_PAREN);
      REQUIRE(tokens[2].type == END);
    }
}