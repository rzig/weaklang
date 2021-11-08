
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

void had_error(std::string str){
    Lexer lex; 
    lex.lex(str); 
    REQUIRE(!lex.has_had_error());
}

TEST_CASE("Simplistic tokens", "[lexer]"){
    SECTION("Simple math"){
      had_error("+");
      had_error("-");
      had_error("*"); 
      had_error("/"); 
      had_error("^"); 
    }
    SECTION("Punctuation"){
      had_error(","); 
      had_error("."); 
      had_error(";"); 
    }
    SECTION("Comments"){
      had_error("#"); 
    }
    SECTION("Parentheses"){
      had_error("("); 
      had_error(")"); 
    }
    SECTION("Block"){
      had_error("{"); 
      had_error("}"); 
    }
    SECTION("Brackets"){
      had_error("["); 
      had_error("]"); 
    }
    SECTION("Assignment"){
      had_error("=");
    }
    SECTION("Boolean"){
      had_error("!"); 
      had_error("=="); 
      had_error(">"); 
      had_error("<");
      had_error(">=");
      had_error("<=");
    }
}