
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

TEST_CASE("Simplistic tokens", "[lexer]"){
    SECTION("Simple math"){
      no_error("+");
      no_error("-");
      no_error("*"); 
      no_error("/"); 
      no_error("^"); 
    }
    SECTION("Punctuation"){
      no_error(","); 
      no_error("."); 
      no_error(";"); 
    }
    SECTION("Comments"){
      no_error("#"); 
    }
    SECTION("Parentheses"){
      no_error("("); 
      no_error(")"); 
    }
    SECTION("Block"){
      no_error("{"); 
      no_error("}"); 
    }
    SECTION("Brackets"){
      no_error("["); 
      no_error("]"); 
    }
    SECTION("Assignment"){
      no_error("=");
    }
    SECTION("Boolean"){
      no_error("!"); 
      no_error("=="); 
      no_error(">"); 
      no_error("<");
      no_error(">=");
      no_error("<=");
    }
    SECTION("String literals") {
      no_error("\"Hello world\"");
    }
}