// This file is part of weak-lang.
// weak-lang is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
// weak-lang is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
// You should have received a copy of the GNU Affero General Public License
// along with weak-lang. If not, see <https://www.gnu.org/licenses/>.

//////////////////////////////////////////////////////////////////////////////
//                             Framework Set-up                             //
//////////////////////////////////////////////////////////////////////////////
#include "catch.hpp"

//////////////////////////////////////////////////////////////////////////////
//                                 Includes                                 //
//////////////////////////////////////////////////////////////////////////////
#include "lexer.hpp"
#include "parser.hpp"
#include "util.hpp"
#include "environment.hpp"
#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>

//////////////////////////////////////////////////////////////////////////////
//                                Lexer tests                               //
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

    SECTION("Accepts numbers after first letter in identifiers") {
        no_error("abc1 = 1");
        expect_tokens("abc1 = 1", {IDENTIFIER, EQUALS, NUMBER, END});
        REQUIRE(getTokens("abc1 = 1")[0].lexeme == "abc1");
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

    SECTION("Assert") {
        no_error("v s m == 2");
        expect_tokens("v s m == 2", {ASSERT, SHAPE, IDENTIFIER, EQUALS_EQUALS, NUMBER, END});
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

    SECTION("non-ascii"){
        Lexer lex; 
        lex.lex("??");
        REQUIRE(lex.has_had_error()); 
    }
}

TEST_CASE("Complex input", "[lexer]") {
    std::ifstream file_in("./tests/test.weak");
    std::stringstream stream;
    stream << file_in.rdbuf();
    file_in.close();
    std::string file_str = stream.str();
    no_error(file_str);
    expect_tokens(file_str, {OPERATOR, IDENTIFIER, LEFT_PAREN, IDENTIFIER, COMMA, IDENTIFIER, RIGHT_PAREN, LEFT_BRACE, RETURN, IDENTIFIER, EXP, LEFT_PAREN, IDENTIFIER, STAR, NUMBER, RIGHT_PAREN, SEMI, RIGHT_BRACE, OPERATOR, IDENTIFIER, LEFT_PAREN, IDENTIFIER, COMMA, IDENTIFIER, RIGHT_PAREN, LEFT_BRACE, RETURN, LEFT_PAREN, SHAPE, IDENTIFIER, RIGHT_PAREN, PLUS, LEFT_PAREN, SHAPE, IDENTIFIER, RIGHT_PAREN, SEMI, RIGHT_BRACE, FUNCTION, IDENTIFIER, LEFT_PAREN, IDENTIFIER, COMMA, IDENTIFIER, COMMA, IDENTIFIER, RIGHT_PAREN, LEFT_BRACE, LET, IDENTIFIER, EQUALS, NUMBER, SEMI, LET, IDENTIFIER, EQUALS, LEFT_BRACK, RIGHT_BRACK, AS_SHAPE, LEFT_BRACK, NUMBER, COMMA, NUMBER, RIGHT_BRACK, SEMI, WHILE, LEFT_PAREN, IDENTIFIER, LESSER, IDENTIFIER, RIGHT_PAREN, LEFT_BRACE, IDENTIFIER, EQUALS, IDENTIFIER, IDENTIFIER, IDENTIFIER, SEMI, IDENTIFIER, EQUALS, IDENTIFIER, PLUS, NUMBER, SEMI, RIGHT_BRACE, IF, LEFT_PAREN, IDENTIFIER, EXCLA_EQUALS, NUMBER, OR, IDENTIFIER, EXCLA_EQUALS, NUMBER, RIGHT_PAREN, LEFT_BRACE, RETURN, TRUE, SEMI, RIGHT_BRACE, RETURN, FALSE, SEMI, RIGHT_BRACE, PRINT, IDENTIFIER, LEFT_PAREN, NUMBER, COMMA, NUMBER, COMMA, NUMBER, RIGHT_PAREN, SEMI, PRINT, STRING, SEMI, PRINT, NIL, SEMI, PRINT, IDENTIFIER, LEFT_PAREN, NUMBER, COMMA, NUMBER, COMMA, NUMBER, RIGHT_PAREN, OR, IDENTIFIER, LEFT_PAREN, NUMBER, COMMA, NUMBER, COMMA, NUMBER, RIGHT_PAREN, SEMI, PRINT, NUMBER, GREATER, NUMBER, SEMI, PRINT, NUMBER, GREATER_EQUALS, NUMBER, SEMI, PRINT, NUMBER, LESSER, NUMBER, SEMI, PRINT, NUMBER, LESSER_EQUALS, NUMBER, SEMI, PRINT, NUMBER, PLUS, NUMBER, MINUS, NUMBER, STAR, NUMBER, SLASH, NUMBER, SEMI, PRINT, LEFT_BRACK, RIGHT_BRACK, AT, LEFT_BRACK, LEFT_PAREN, NUMBER, STAR, NUMBER, PLUS, IDENTIFIER, RIGHT_PAREN, COMMA, NUMBER, RIGHT_BRACK, SEMI, PRINT, IDENTIFIER, IDENTIFIER, IDENTIFIER, SEMI, IDENTIFIER, EQUALS, IDENTIFIER, EQUALS, IDENTIFIER, STAR, IDENTIFIER, PLUS, IDENTIFIER, AT, IDENTIFIER, LEFT_BRACK, NUMBER, COMMA, NUMBER, COMMA, MINUS, NUMBER, COMMA, NUMBER, COMMA, NUMBER, RIGHT_BRACK, SEMI, END});
    // Now we'll spot-check a few tokens for correct row/column data
    // (doing so for all nearly 200 would be excessive)
    std::vector<Token> tokens = getTokens(file_str);
    REQUIRE(tokens[50].col == 7);
    REQUIRE(tokens[50].line == 10);
    REQUIRE(tokens[100].col == 3);
    REQUIRE(tokens[100].line == 19);
    REQUIRE(tokens[150].col == 3);
    REQUIRE(tokens[150].line == 28);
}

//////////////////////////////////////////////////////////////////////////////
//                               Parser tests                               //
//////////////////////////////////////////////////////////////////////////////

#define REQUIRE_ABILITY(x) if(x) {REQUIRE(true);} else {REQUIRE(false);}
#define TO_MAKE(type, variable) CAN_MAKE(type, variable)
#define required_if(condition) REQUIRE_ABILITY(condition); if(condition)

std::vector<Stmt*> getStatements(std::string program) {
    Lexer lex;
    Parser p {lex.lex(program)};
    return p.parse();
}

TEST_CASE("Operation", "[parser]") {
    SECTION("with single or") {
        auto statements = getStatements("var O T;");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(e->expr)) {
                REQUIRE(b->op.type == OR);
                required_if(CAN_MAKE(Var*, v)_FROM(b->left)) {
                    REQUIRE(v->name.lexeme == "var");
                }
                required_if(CAN_MAKE(Literal*, l)_FROM(b->right)) {
                    REQUIRE(l->literal_type == LiteralType::LITERAL_BOOL);
                    REQUIRE(l->bool_val == true);
                }
            }
        }
    }

    SECTION("with single and") {
        auto statements = getStatements("func() A arr[1];");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(e->expr)) {
                REQUIRE(b->op.type == AND);
                required_if(CAN_MAKE(Func*, f)_FROM(b->left)) {
                    REQUIRE(f->func.lexeme == "func");
                    REQUIRE(f->args.size() == 0);
                }
                required_if(CAN_MAKE(ArrAccess*, a)_FROM(b->right)) {
                    required_if(CAN_MAKE(Var*, v)_FROM(a->id)) {
                        REQUIRE(v->name.lexeme == "arr");
                    }
                    REQUIRE(a->idx.size() == 1);
                    required_if(CAN_MAKE(Literal*, l)_FROM(a->idx[0])) {
                        REQUIRE(l->literal_type == LiteralType::LITERAL_DOUBLE);
                        REQUIRE(l->double_val == 1);
                    }
                }
            }
        }
    }

    SECTION("with multiple and/or") {
        // The purpose of this test is solely to test order of
        // operations and *not* literal parsing, so we do not check
        // that the literal values are correct, only that they exist
        auto statements = getStatements("T O F A T;");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, parent)_FROM(e->expr)) {
                REQUIRE_ABILITY(TO_MAKE(Literal*, l)_FROM(parent->left));
                REQUIRE(parent->op.type == OR);
                required_if(CAN_MAKE(Binary*, child)_FROM(parent->right)) {
                    REQUIRE(child->op.type == AND);
                    REQUIRE_ABILITY(TO_MAKE(Literal*, cl1)_FROM(child->left));
                    REQUIRE_ABILITY(TO_MAKE(Literal*, cl2)_FROM(child->right));
                }
            }
        }
    }

    SECTION("Error - hanging O") {
        REQUIRE_THROWS_WITH(getStatements("T O F O;"), "Expected primary but instead found: \";\", at line 1 and column 8, this token has type SEMI");
    }

    SECTION("Error - hanging A") {
        REQUIRE_THROWS_WITH(getStatements("T O F A;"), "Expected primary but instead found: \";\", at line 1 and column 8, this token has type SEMI");
    }
}

TEST_CASE("Equality", "[parser]") {
    SECTION("with two literals and ==") {
        auto statements = getStatements("T == 1;");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(e->expr)) {
                required_if(CAN_MAKE(Literal*, l)_FROM(b->left)) {
                    REQUIRE(l->literal_type == LiteralType::LITERAL_BOOL);
                    REQUIRE(l->bool_val == true);
                }
                required_if(CAN_MAKE(Literal*, r)_FROM(b->right)) {
                    REQUIRE(r->literal_type == LITERAL_DOUBLE);
                    REQUIRE(r->double_val == 1);
                }
                REQUIRE(b->op.type == EQUALS_EQUALS);
            }
        }
    }

    SECTION("with function and variable and ==") {
        auto statements = getStatements("k(b) == b;");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(e->expr)) {
                required_if(CAN_MAKE(Func*, f)_FROM(b->left)) {
                    REQUIRE(f->func.lexeme == "k");
                    REQUIRE(f->args.size() == 1);
                    required_if(CAN_MAKE(Var*, v)_FROM(f->args[0])) {
                        REQUIRE(v->name.lexeme == "b");
                    }
                }
                required_if(CAN_MAKE(Var*, v)_FROM(b->right)) {
                    REQUIRE(v->name.lexeme == "b");
                }
                REQUIRE(b->op.type == EQUALS_EQUALS);
            }
        }
    }

    SECTION("with literal and variable and !=") {
        auto statements = getStatements("k != 10;");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(e->expr)) {
                required_if(CAN_MAKE(Var*, v)_FROM(b->left)) {
                    REQUIRE(v->name.lexeme == "k");
                }
                required_if(CAN_MAKE(Literal*, l)_FROM(b->right)) {
                    REQUIRE(l->literal_type == LiteralType::LITERAL_DOUBLE);
                    REQUIRE(l->double_val == 10);
                }
                REQUIRE(b->op.type == EXCLA_EQUALS);
            }
        }
    }

    SECTION("Error - hanging ==") {
        REQUIRE_THROWS_WITH(getStatements("T == "), "Expected primary but instead found: \"\", at line 1 and column 6, this token has type END");
    }

    SECTION("Error - hanging !=") {
        REQUIRE_THROWS_WITH(getStatements("F != "), "Expected primary but instead found: \"\", at line 1 and column 6, this token has type END");
    }
}

TEST_CASE("Comparison", "[parser]") {
    SECTION("literal > literal") {
        auto statements = getStatements("2 > 3;");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(e->expr)) {
                required_if(CAN_MAKE(Literal*, left)_FROM(b->left)) {
                    REQUIRE(left->literal_type == LiteralType::LITERAL_DOUBLE);
                    REQUIRE(left->double_val == 2);
                }
                required_if(CAN_MAKE(Literal*, right)_FROM(b->right)) {
                    REQUIRE(right->literal_type == LiteralType::LITERAL_DOUBLE);
                    REQUIRE(right->double_val == 3);
                }
                REQUIRE(b->op.type == GREATER);
            }
        }
    }

    SECTION("variable >= function") {
        auto statements = getStatements("var >= func(b,2);");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(e->expr)) {
                required_if(CAN_MAKE(Var*, v)_FROM(b->left)) {
                    REQUIRE(v->name.lexeme == "var");
                }
                required_if(CAN_MAKE(Func*, f)_FROM(b->right)) {
                    REQUIRE(f->func.lexeme == "func");
                    REQUIRE(f->args.size() == 2);
                    required_if(CAN_MAKE(Var*, v)_FROM(f->args[0])) {
                        REQUIRE(v->name.lexeme == "b");
                    }
                    required_if(CAN_MAKE(Literal*, l)_FROM(f->args[1])) {
                        REQUIRE(l->literal_type == LiteralType::LITERAL_DOUBLE);
                        REQUIRE(l->double_val == 2);
                    }
                }
                REQUIRE(b->op.type == GREATER_EQUALS);
            }
        }
    }

    SECTION("function <= literal") {
        auto statements = getStatements("t() <= 3;");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(e->expr)) {
                required_if(CAN_MAKE(Func*, f)_FROM(b->left)) {
                    REQUIRE(f->func.lexeme == "t");
                    REQUIRE(f->args.size() == 0);
                }
                required_if(CAN_MAKE(Literal*, l)_FROM(b->right)) {
                    REQUIRE(l->literal_type == LiteralType::LITERAL_DOUBLE);
                    REQUIRE(l->double_val == 3);
                }
                REQUIRE(b->op.type == LESSER_EQUALS);
            }
        }
    }

    SECTION("arrAccess < unary") {
        auto statements = getStatements("arr[1] < -x;");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(e->expr)) {
                required_if(CAN_MAKE(ArrAccess*, a)_FROM(b->left)) {
                    required_if(CAN_MAKE(Var*, v)_FROM(a->id)) {
                        REQUIRE(v->name.lexeme == "arr");
                    }
                    REQUIRE(a->idx.size() == 1);
                    required_if(CAN_MAKE(Literal*, l)_FROM(a->idx[0])) {
                        REQUIRE(l->literal_type == LiteralType::LITERAL_DOUBLE);
                        REQUIRE(l->double_val == 1);
                    }
                }
                required_if(CAN_MAKE(Unary*, u)_FROM(b->right)) {
                    REQUIRE(u->op.type == MINUS);
                    required_if(CAN_MAKE(Var*, v)_FROM(u->right)) {
                        REQUIRE(v->name.lexeme == "x");
                    }
                }
                REQUIRE(b->op.type == LESSER);
            }
        }
    }

    SECTION("Error - hanging >") {
        REQUIRE_THROWS_WITH(getStatements("q > "), "Expected primary but instead found: \"\", at line 1 and column 5, this token has type END");
    }

    SECTION("Error - hanging >=") {
        REQUIRE_THROWS_WITH(getStatements("t >= "), "Expected primary but instead found: \"\", at line 1 and column 6, this token has type END");
    }

    SECTION("Error - hanging <=") {
        REQUIRE_THROWS_WITH(getStatements("t <= "), "Expected primary but instead found: \"\", at line 1 and column 6, this token has type END");
    }

    SECTION("Error - hanging <") {
        REQUIRE_THROWS_WITH(getStatements("u < "), "Expected primary but instead found: \"\", at line 1 and column 5, this token has type END");
    }
}

TEST_CASE("Terms and factors", "[parser]") {
    SECTION("multiple addition and subtraction") {
        auto statements = getStatements("d + b - c + 3;");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, parent)_FROM(e->expr)) {
                REQUIRE(parent->op.type == PLUS);
                required_if(CAN_MAKE(Literal*, three)_FROM(parent->right)) {
                    REQUIRE(three->literal_type == LiteralType::LITERAL_DOUBLE);
                    REQUIRE(three->double_val == 3);
                }
                required_if(CAN_MAKE(Binary*, child1)_FROM(parent->left)) {
                    REQUIRE(child1->op.type == MINUS);
                    required_if(CAN_MAKE(Var*, c)_FROM(child1->right)) {
                        REQUIRE(c->name.lexeme == "c");
                    }
                    required_if(CAN_MAKE(Binary*, child2)_FROM(child1->left)) {
                        REQUIRE(child2->op.type == PLUS);
                        required_if(CAN_MAKE(Var*, d)_FROM(child2->left)) {
                            REQUIRE(d->name.lexeme == "d");
                        }
                        required_if(CAN_MAKE(Var*, b)_FROM(child2->right)) {
                            REQUIRE(b->name.lexeme == "b");
                        }
                    }
                }
            }
        }
    }

    SECTION("multiplication and division with minus") {
        auto statements = getStatements("d * b - c / 3;");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, parent)_FROM(e->expr)) {
                REQUIRE(parent->op.type == MINUS);
                required_if(CAN_MAKE(Binary*, left)_FROM(parent->left)) {
                    required_if(CAN_MAKE(Var*, d)_FROM(left->left)) {
                        REQUIRE(d->name.lexeme == "d");
                    }
                    required_if(CAN_MAKE(Var*, b)_FROM(left->right)) {
                        REQUIRE(b->name.lexeme == "b");
                    }
                    REQUIRE(left->op.type == STAR);
                }
                required_if(CAN_MAKE(Binary*, right)_FROM(parent->right)) {
                    required_if(CAN_MAKE(Var*, c)_FROM(right->left)) {
                        REQUIRE(c->name.lexeme == "c");
                    }
                    required_if(CAN_MAKE(Literal*, three)_FROM(right->right)) {
                        REQUIRE(three->literal_type == LiteralType::LITERAL_DOUBLE);
                        REQUIRE(three->double_val == 3);
                    }
                    REQUIRE(right->op.type == SLASH);
                }
            }
        }
    }

    SECTION("all binary operators with a unary operator") {
        auto statements = getStatements("(b@c) + (c/d) * (d^e) * s arr;");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, parent)_FROM(e->expr)) {
                REQUIRE(parent->op.type == PLUS);
                required_if(CAN_MAKE(Binary*, child1)_FROM(parent->left)) {
                    REQUIRE(child1->op.type == AT);
                    required_if(CAN_MAKE(Var*, b)_FROM(child1->left)) {
                        REQUIRE(b->name.lexeme == "b");
                    }
                    required_if(CAN_MAKE(Var*, c)_FROM(child1->right)) {
                        REQUIRE(c->name.lexeme == "c");
                    }
                }
                required_if(CAN_MAKE(Binary*, child2)_FROM(parent->right)) {
                    REQUIRE(child2->op.type == STAR);
                    required_if(CAN_MAKE(Unary*, u)_FROM(child2->right)) {
                        REQUIRE(u->op.type == SHAPE);
                        required_if(CAN_MAKE(Var*, arr)_FROM(u->right)) {
                            REQUIRE(arr->name.lexeme == "arr");
                        }
                    }
                    required_if(CAN_MAKE(Binary*, child3)_FROM(child2->left)) {
                        REQUIRE(child3->op.type == STAR);
                        required_if(CAN_MAKE(Binary*, child4)_FROM(child3->left)) {
                            REQUIRE(child4->op.type == SLASH);
                            required_if(CAN_MAKE(Var*, c)_FROM(child4->left)) {
                                REQUIRE(c->name.lexeme == "c");
                            }
                            required_if(CAN_MAKE(Var*, d)_FROM(child4->right)) {
                                REQUIRE(d->name.lexeme == "d");
                            }
                        }
                        required_if(CAN_MAKE(Binary*, child5)_FROM(child3->right)) {
                            REQUIRE(child5->op.type == EXP);
                            required_if(CAN_MAKE(Var*, d)_FROM(child5->left)) {
                                REQUIRE(d->name.lexeme == "d");
                            }
                            required_if(CAN_MAKE(Var*, e)_FROM(child5->right)) {
                                REQUIRE(e->name.lexeme == "e");
                            }
                        }
                    }
                }
            }
        }
    }

    SECTION("Error - hanging +") {
        REQUIRE_THROWS_WITH(getStatements("b + "), "Expected primary but instead found: \"\", at line 1 and column 5, this token has type END");
    }

    SECTION("Error - hanging -") {
        REQUIRE_THROWS_WITH(getStatements("c - "), "Expected primary but instead found: \"\", at line 1 and column 5, this token has type END");
    }

    SECTION("Error - hanging *") {
        REQUIRE_THROWS_WITH(getStatements("c - b *"), "Expected primary but instead found: \"\", at line 1 and column 8, this token has type END");
    }

    SECTION("Error - hanging /") {
        REQUIRE_THROWS_WITH(getStatements("c / "), "Expected primary but instead found: \"\", at line 1 and column 5, this token has type END");
    }

    SECTION("Error - hanging @") {
        REQUIRE_THROWS_WITH(getStatements("d / "), "Expected primary but instead found: \"\", at line 1 and column 5, this token has type END");
    }

    SECTION("Error - nonsense after operator") {
        REQUIRE_THROWS_WITH(getStatements("d / a"), "Expected primary but instead found: \"a\", at line 1 and column 5, this token has type LET");
    }
}

TEST_CASE("Unary operations", "[parser]") {
    SECTION("!") {
        auto statements = getStatements("!var;");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Unary*, u)_FROM(e->expr)) {
                REQUIRE(u->op.type == EXCLA);
                required_if(CAN_MAKE(Var*, var)_FROM(u->right)) {
                    REQUIRE(var->name.lexeme == "var");
                }
            }
        }
    }

    SECTION("-") {
        auto statements = getStatements("-x;");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Unary*, u)_FROM(e->expr)) {
                REQUIRE(u->op.type == MINUS);
                required_if(CAN_MAKE(Var*, x)_FROM(u->right)) {
                    REQUIRE(x->name.lexeme == "x");
                }
            }
        }
    }

    SECTION("s") {
        auto statements = getStatements("s (c + d);");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Unary*, u)_FROM(e->expr)) {
                REQUIRE(u->op.type == SHAPE);
                required_if(CAN_MAKE(Binary*, b)_FROM(u->right)) {
                    REQUIRE(b->op.type == PLUS);
                    required_if(CAN_MAKE(Var*, c)_FROM(b->left)) {
                        REQUIRE(c->name.lexeme == "c");
                    }
                    required_if(CAN_MAKE(Var*, d)_FROM(b->right)) {
                        REQUIRE(d->name.lexeme == "d");
                    }
                }
            }
        }
    }

    SECTION("Error - hanging !") {
        REQUIRE_THROWS_WITH(getStatements("!"), "Expected primary but instead found: \"\", at line 1 and column 2, this token has type END");
    }

    SECTION("Error - hanging -") {
        REQUIRE_THROWS_WITH(getStatements("-"), "Expected primary but instead found: \"\", at line 1 and column 2, this token has type END");
    }

    SECTION("Error - hanging s") {
        REQUIRE_THROWS_WITH(getStatements("s"), "Expected primary but instead found: \"\", at line 1 and column 2, this token has type END");
    }

    SECTION("Error - nonsense after operator") {
        REQUIRE_THROWS_WITH(getStatements("- o;"), "Expected primary but instead found: \"o\", at line 1 and column 3, this token has type OPERATOR");
    }
}

TEST_CASE("Array access", "[parser]") {
    SECTION("1D") {
        auto statements = getStatements("array[1];");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(ArrAccess*, a)_FROM(e->expr)) {
                required_if(CAN_MAKE(Var*, v)_FROM(a->id)) {
                    REQUIRE(v->name.lexeme == "array");
                }
                REQUIRE(a->idx.size() == 1);
                required_if(CAN_MAKE(Literal*, one)_FROM(a->idx[0])) {
                    REQUIRE(one->literal_type == LiteralType::LITERAL_DOUBLE);
                    REQUIRE(one->double_val == 1);
                }
            }
        }
    }

    SECTION("2D") {
        auto statements = getStatements("mat[1, b];");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(ArrAccess*, a)_FROM(e->expr)) {
                required_if(CAN_MAKE(Var*, v)_FROM(a->id)) {
                    REQUIRE(v->name.lexeme == "mat");
                }
                REQUIRE(a->idx.size() == 2);
                required_if(CAN_MAKE(Literal*, one)_FROM(a->idx[0])) {
                    REQUIRE(one->literal_type == LiteralType::LITERAL_DOUBLE);
                    REQUIRE(one->double_val == 1);
                }
                required_if(CAN_MAKE(Var*, b)_FROM(a->idx[1])) {
                    REQUIRE(b->name.lexeme == "b");
                }
            }
        }
    }

    SECTION("Error - empty access") {
        REQUIRE_THROWS_WITH(getStatements("arr[];"), "Expected primary but instead found: \"]\", at line 1 and column 5, this token has type RIGHT_BRACK");
    }

    SECTION("Error - missing comma") {
        REQUIRE_THROWS_WITH(getStatements("arr[1 2];"), "Expected comma in array indexing but instead found: \"2\", at line 1 and column 7, this token has type NUMBER");
    }
}

// Note: for the above cases, we do not test for semicolons because it is reasonable to assume
// that these expressions will be used in things like while conditions etc.

TEST_CASE("While statements", "[parser]") {
    SECTION("with simple expression") {
        auto statements = getStatements("w (var) {var = var O T;}");
        required_if(CAN_MAKE(While*, w)_FROM(statements[0])) {
            required_if(CAN_MAKE(Var*, var)_FROM(w->cond)) {
                REQUIRE(var->name.lexeme == "var");
            }
            REQUIRE(w->stmts.size() == 1);
            required_if(CAN_MAKE(ExprStmt*, e)_FROM(w->stmts[0])) {
                required_if(CAN_MAKE(Assign*, a)_FROM(e->expr)) {
                    REQUIRE(a->name.lexeme == "var");
                    required_if(CAN_MAKE(Binary*, b)_FROM(a->value)) {
                        REQUIRE(b->op.type == OR);
                        required_if(CAN_MAKE(Var*, var)_FROM(b->left)) {
                            REQUIRE(var->name.lexeme == "var");
                        }
                        required_if(CAN_MAKE(Literal*, t)_FROM(b->right)) {
                            REQUIRE(t->literal_type == LiteralType::LITERAL_BOOL);
                            REQUIRE(t->bool_val == true);
                        }
                    }
                }
            }
        }
    }

    SECTION("with complex expression") {
        auto statements = getStatements("w (var A !func(var)) {}");
        required_if(CAN_MAKE(While*, w)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(w->cond)) {
                REQUIRE(b->op.type == AND);
                required_if(CAN_MAKE(Var*, var)_FROM(b->left)) {
                    REQUIRE(var->name.lexeme == "var");
                }
                required_if(CAN_MAKE(Unary*, u)_FROM(b->right)) {
                    REQUIRE(u->op.type == EXCLA);
                    required_if(CAN_MAKE(Func*, func)_FROM(u->right)) {
                        REQUIRE(func->func.lexeme == "func");
                        REQUIRE(func->args.size() == 1);
                        required_if(CAN_MAKE(Var*, var)_FROM(func->args[0])) {
                            REQUIRE(var->name.lexeme == "var");
                        }
                    }
                }
            }
            REQUIRE(w->stmts.size() == 0);
        }
    }

    SECTION("Error - without condition") {
        REQUIRE_THROWS_WITH(getStatements("w {}"), "Expected '(' before condition but instead found: \"{\", at line 1 and column 3, this token has type LEFT_BRACE");
    }

    SECTION("Error - empty condition") {
        REQUIRE_THROWS_WITH(getStatements("w () {}"), "Expected primary but instead found: \")\", at line 1 and column 4, this token has type RIGHT_PAREN");
    }

    SECTION("Error - without block") {
        REQUIRE_THROWS_WITH(getStatements("w (T)"), "Expected '{' before contents of while loop but instead found: \"\", at line 1 and column 6, this token has type END");
    }

    SECTION("Error - unterminated block") {
        REQUIRE_THROWS_WITH(getStatements("w (T) {"), "Expected '}' after block but instead found: \"\", at line 1 and column 8, this token has type END");
    }
}

TEST_CASE("Return statement", "[parser]") {
    SECTION("with no expression") {
        auto statements = getStatements("r;");
        required_if(CAN_MAKE(Return*, r)_FROM(statements[0])) {
            REQUIRE_ABILITY(TO_MAKE(Nil*, n)_FROM(r->expr));
        }
    }

    SECTION("with expression") {
        auto statements = getStatements("r var + 2;");
        required_if(CAN_MAKE(Return*, r)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(r->expr)) {
                required_if(CAN_MAKE(Var*, var)_FROM(b->left)) {
                    REQUIRE(var->name.lexeme == "var");
                }
                required_if(CAN_MAKE(Literal*, two)_FROM(b->right)) {
                    REQUIRE(two->literal_type == LiteralType::LITERAL_DOUBLE);
                    REQUIRE(two->double_val == 2);
                }
                REQUIRE(b->op.type == PLUS);
            }
        }
    }

    SECTION("Error - no semicolon after blank return") {
        REQUIRE_THROWS_WITH(getStatements("r"), "Expected primary but instead found: \"\", at line 1 and column 2, this token has type END");
    }

    SECTION("Error - no semicolon after expression return") {
        REQUIRE_THROWS_WITH(getStatements("r T"), "Expected ';' after return statement but instead found: \"\", at line 1 and column 4, this token has type END");
    }
}

TEST_CASE("Print statement", "[parser]") {
    SECTION("with simple expression") {
        auto statements = getStatements("p it;");
        required_if(CAN_MAKE(Print*, p)_FROM(statements[0])) {
            required_if(CAN_MAKE(Var*, it)_FROM(p->expr)) {
                REQUIRE(it->name.lexeme == "it");
            }
        }
    }

    SECTION("with complex expression") {
        auto statements = getStatements("p l() + b@b;");
        required_if(CAN_MAKE(Print*, p)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, parent)_FROM(p->expr)) {
                required_if(CAN_MAKE(Func*, l)_FROM(parent->left)) {
                    REQUIRE(l->func.lexeme == "l");
                    REQUIRE(l->args.size() == 0);
                }
                required_if(CAN_MAKE(Binary*, child)_FROM(parent->right)) {
                    required_if(CAN_MAKE(Var*, b)_FROM(child->left)) {
                        REQUIRE(b->name.lexeme == "b");
                    }
                    required_if(CAN_MAKE(Var*, b)_FROM(child->right)) {
                        REQUIRE(b->name.lexeme == "b");
                    }
                    REQUIRE(child->op.type == AT);
                }
                REQUIRE(parent->op.type == PLUS);
            }
        }
    }

    SECTION("Error - no semicolon after print") {
        REQUIRE_THROWS_WITH(getStatements("p T"), "Expected ';' after print statement but instead found: \"\", at line 1 and column 4, this token has type END");
    }

    SECTION("Error - no expression after print") {
        REQUIRE_THROWS_WITH(getStatements("p ;"), "Expected primary but instead found: \";\", at line 1 and column 3, this token has type SEMI");
    }
}

TEST_CASE("Assert statement", "[parser]") {
    SECTION("with simple expression") {
        auto statements = getStatements("v b == c;");
        required_if(CAN_MAKE(Assert*, a)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(a->cond)) {
                required_if(CAN_MAKE(Var*, left)_FROM(b->left)) {
                    REQUIRE(left->name.lexeme == "b");
                }
                required_if(CAN_MAKE(Var*, right)_FROM(b->right)) {
                    REQUIRE(right->name.lexeme == "c");
                }
                REQUIRE(b->op.type == EQUALS_EQUALS);
            }
        }
    }

    SECTION("with complex expression") {
        auto statements = getStatements("v func() O var O array[1];");
        required_if(CAN_MAKE(Assert*, assert_s)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, parent)_FROM(assert_s->cond)) {
                required_if(CAN_MAKE(ArrAccess*, arr)_FROM(parent->right)) {
                    required_if(CAN_MAKE(Var*, v)_FROM(arr->id)) {
                        REQUIRE(v->name.lexeme == "array");
                    }
                    REQUIRE(arr->idx.size() == 1);
                    required_if(CAN_MAKE(Literal*, idx)_FROM(arr->idx[0])) {
                        REQUIRE(idx->literal_type == LITERAL_DOUBLE);
                        REQUIRE(idx->double_val == 1);
                    }
                }
                REQUIRE(parent->op.type == OR);
                required_if(CAN_MAKE(Binary*, child)_FROM(parent->left)) {
                    required_if(CAN_MAKE(Func*, func)_FROM(child->left)) {
                        REQUIRE(func->args.size() == 0);
                        REQUIRE(func->func.lexeme == "func");
                    }
                    REQUIRE(child->op.type == OR);
                    required_if(CAN_MAKE(Var*, v)_FROM(child->right)) {
                        REQUIRE(v->name.lexeme == "var");
                    }
                }
            }
        }
    }

    SECTION("Error - no semicolon after assert") {
        REQUIRE_THROWS_WITH(getStatements("v T"), "Expected ';' after assert statement but instead found: \"\", at line 1 and column 4, this token has type END");
    }

    SECTION("Error - no condition after assert") {
        REQUIRE_THROWS_WITH(getStatements("v ;"), "Expected primary but instead found: \";\", at line 1 and column 3, this token has type SEMI");
    }
}

TEST_CASE("If statement", "[parser]") {
    SECTION("with simple expression") {
        auto statements = getStatements("i (T) {}");
        required_if(CAN_MAKE(If*, i)_FROM(statements[0])) {
            required_if(CAN_MAKE(Literal*, t)_FROM(i->cond)) {
                REQUIRE(t->literal_type == LiteralType::LITERAL_BOOL);
                REQUIRE(t->bool_val == true);
            }
            REQUIRE(i->stmts.size() == 0);
        }
    }

    SECTION("with complex expression") {
        auto statements = getStatements("i (var O !func(var)) {b = c; c = b; var = b == c;}");
        required_if(CAN_MAKE(If*, i)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(i->cond)) {
                REQUIRE(b->op.type == OR);
                required_if(CAN_MAKE(Var*, var)_FROM(b->left)) {
                    REQUIRE(var->name.lexeme == "var");
                }
                required_if(CAN_MAKE(Unary*, u)_FROM(b->right)) {
                    REQUIRE(u->op.type == EXCLA);
                    required_if(CAN_MAKE(Func*, func)_FROM(u->right)) {
                        REQUIRE(func->func.lexeme == "func");
                        REQUIRE(func->args.size() == 1);
                        required_if(CAN_MAKE(Var*, var)_FROM(func->args[0])) {
                            REQUIRE(var->name.lexeme == "var");
                        }
                    }
                }
            }
            REQUIRE(i->stmts.size() == 3);
            required_if(CAN_MAKE(ExprStmt*, e1)_FROM(i->stmts[0])) {
                required_if(CAN_MAKE(Assign*, a)_FROM(e1->expr)) {
                    REQUIRE(a->name.lexeme == "b");
                    required_if(CAN_MAKE(Var*, c)_FROM(a->value)) {
                        REQUIRE(c->name.lexeme == "c");
                    }
                }
            }
            required_if(CAN_MAKE(ExprStmt*, e2)_FROM(i->stmts[1])) {
                required_if(CAN_MAKE(Assign*, a)_FROM(e2->expr)) {
                    REQUIRE(a->name.lexeme == "c");
                    required_if(CAN_MAKE(Var*, b)_FROM(a->value)) {
                        REQUIRE(b->name.lexeme == "b");
                    }
                }
            }
            required_if(CAN_MAKE(ExprStmt*, e3)_FROM(i->stmts[2])) {
                required_if(CAN_MAKE(Assign*, a)_FROM(e3->expr)) {
                    REQUIRE(a->name.lexeme == "var");
                    required_if(CAN_MAKE(Binary*, bin)_FROM(a->value)) {
                        required_if(CAN_MAKE(Var*, b)_FROM(bin->left)) {
                            REQUIRE(b->name.lexeme == "b");
                        }
                        required_if(CAN_MAKE(Var*, c)_FROM(bin->right)) {
                            REQUIRE(c->name.lexeme == "c");
                        }
                        REQUIRE(bin->op.type == EQUALS_EQUALS);
                    }
                }
            }
        }
    }

    SECTION("Error - without condition") {
        REQUIRE_THROWS_WITH(getStatements("i {}"), "Expected '(' before condition but instead found: \"{\", at line 1 and column 3, this token has type LEFT_BRACE");
    }

    SECTION("Error - empty condition") {
        REQUIRE_THROWS_WITH(getStatements("i () {}"), "Expected primary but instead found: \")\", at line 1 and column 4, this token has type RIGHT_PAREN");
    }

    SECTION("Error - without block") {
        REQUIRE_THROWS_WITH(getStatements("i (T)"), "Expected '{' before contents of if statement but instead found: \"\", at line 1 and column 6, this token has type END");
    }

    SECTION("Error - unterminated block") {
        REQUIRE_THROWS_WITH(getStatements("i (F) {"), "Expected '}' after block but instead found: \"\", at line 1 and column 8, this token has type END");
    }
}

TEST_CASE("Function declaration", "[parser]") {
    SECTION("With no parameters") {
        auto statements = getStatements("f x() {r T;}");
        required_if(CAN_MAKE(FuncDecl*, f)_FROM(statements[0])) {
            REQUIRE(f->name.lexeme == "x");
            REQUIRE(f->params.size() == 0);
            REQUIRE(f->stmts.size() == 1);
            required_if(CAN_MAKE(Return*, r)_FROM(f->stmts[0])) {
                required_if(CAN_MAKE(Literal*, l)_FROM(r->expr)) {
                    REQUIRE(l->literal_type == LiteralType::LITERAL_BOOL);
                    REQUIRE(l->bool_val == true);
                }
            }
        }
    }

    SECTION("With one parameter") {
        auto statements = getStatements("f y(t) {r t;}");
        required_if(CAN_MAKE(FuncDecl*, f)_FROM(statements[0])) {
            REQUIRE(f->name.lexeme == "y");
            REQUIRE(f->params.size() == 1);
            REQUIRE(f->params[0].lexeme == "t");
            REQUIRE(f->stmts.size() == 1);
            required_if(CAN_MAKE(Return*, r)_FROM(f->stmts[0])) {
                required_if(CAN_MAKE(Var*, t)_FROM(r->expr)) {
                    REQUIRE(t->name.lexeme == "t");
                }
            }
        }
    }

    SECTION("With two parameters") {
        auto statements = getStatements("f z(t, k) {r t + k;}");
        required_if(CAN_MAKE(FuncDecl*, f)_FROM(statements[0])) {
            REQUIRE(f->name.lexeme == "z");
            REQUIRE(f->params.size() == 2);
            REQUIRE(f->params[0].lexeme == "t");
            REQUIRE(f->params[1].lexeme == "k");
            REQUIRE(f->stmts.size() == 1);
            required_if(CAN_MAKE(Return*, r)_FROM(f->stmts[0])) {
                required_if(CAN_MAKE(Binary*, b)_FROM(r->expr)) {
                    required_if(CAN_MAKE(Var*, t)_FROM(b->left)) {
                        REQUIRE(t->name.lexeme == "t");
                    }
                    required_if(CAN_MAKE(Var*, k)_FROM(b->right)) {
                        REQUIRE(k->name.lexeme == "k");
                    }
                    REQUIRE(b->op.type == PLUS);
                }
            }
        }
    }

    SECTION("Error - missing commas") {
        REQUIRE_THROWS_WITH(getStatements("f x(b c) {}"), "Expected comma in function declaration but instead found: \"c\", at line 1 and column 7, this token has type IDENTIFIER");
    }

    SECTION("Error - missing block") {
        REQUIRE_THROWS_WITH(getStatements("f x(b, c)"), "Expected '{' before function body but instead found: \"\", at line 1 and column 10, this token has type END");
    }

    SECTION("Error - nonsense parameters") {
        REQUIRE_THROWS_WITH(getStatements("f x(a)"), "Expected parameter name in function declaration but instead found: \"a\", at line 1 and column 5, this token has type LET");
    }

    SECTION("Error - unterminated block") {
        REQUIRE_THROWS_WITH(getStatements("f x(b) {"), "Expected '}' after block but instead found: \"\", at line 1 and column 9, this token has type END");
    }
}

TEST_CASE("Operator declaration", "[parser]") {
    // There is only one valid way to create an operator
    SECTION("Standard declaration") {
        auto statements = getStatements("o n(x, y) {}");
        required_if(CAN_MAKE(OpDecl*, o)_FROM(statements[0])) {
            REQUIRE(o->name.lexeme == "n");
            REQUIRE(o->left.lexeme == "x");
            REQUIRE(o->right.lexeme == "y");
            REQUIRE(o->stmts.size() == 0);
        }
    }

    SECTION("Error - no parameters") {
        REQUIRE_THROWS_WITH(getStatements("o x() {}"), "Exepcted left parameter name in operator declaration but instead found: \")\", at line 1 and column 5, this token has type RIGHT_PAREN");
    }

    SECTION("Error - one parameter") {
        REQUIRE_THROWS_WITH(getStatements("o x(b) {}"), "Exepcted ',' after left operator parameter but instead found: \")\", at line 1 and column 6, this token has type RIGHT_PAREN");
    }

    SECTION("Error - three parameters") {
        REQUIRE_THROWS_WITH(getStatements("o x(b,c,d) {}"), "Expected ')' after operator parameters list but instead found: \",\", at line 1 and column 8, this token has type COMMA");
    }

    SECTION("Error - no comma") {
        REQUIRE_THROWS_WITH(getStatements("o x(b c) {}"), "Exepcted ',' after left operator parameter but instead found: \"c\", at line 1 and column 7, this token has type IDENTIFIER");
    }

    SECTION("Error - no block") {
        REQUIRE_THROWS_WITH(getStatements("o x(b, c)"), "Expected '{' before operator body but instead found: \"\", at line 1 and column 10, this token has type END");
    }

    SECTION("Error - unterminated block") {
        REQUIRE_THROWS_WITH(getStatements("o x(b,c) {"), "Expected '}' after block but instead found: \"\", at line 1 and column 11, this token has type END");
    }
}

TEST_CASE("Variable declaration", "[parser]"){
    SECTION("to literal") {
        auto statements = getStatements("a x = 5;");
        required_if(CAN_MAKE(VarDecl*, v)_FROM(statements[0])) {
            REQUIRE(v->name.lexeme == "x");
            required_if(CAN_MAKE(Literal*, l)_FROM(v->expr)) {
                REQUIRE(l->literal_type == LiteralType::LITERAL_DOUBLE);
                REQUIRE(l->double_val == 5);
            }
        }
    }

    SECTION("to function") {
        auto statements = getStatements("a var = func(b, c);");
        required_if(CAN_MAKE(VarDecl*, v)_FROM(statements[0])) {
            REQUIRE(v->name.lexeme == "var");
            required_if(CAN_MAKE(Func*, f)_FROM(v->expr)) {
                REQUIRE(f->func.lexeme == "func");
                REQUIRE(f->args.size() == 2);
                required_if(CAN_MAKE(Var*, v1)_FROM(f->args[0])) {
                    REQUIRE(v1->name.lexeme == "b");
                }
                required_if(CAN_MAKE(Var*, v2)_FROM(f->args[1])) {
                    REQUIRE(v2->name.lexeme == "c");
                }
            }
        }
    }

    SECTION ("to array") {
        auto statements = getStatements("a array = [1,2,3];");
        required_if(CAN_MAKE(VarDecl*, v)_FROM(statements[0])) {
            REQUIRE(v->name.lexeme == "array");
            required_if(CAN_MAKE(Literal*, l)_FROM(v->expr)) {
                REQUIRE(l->literal_type == LiteralType::LITERAL_ARRAY);
                REQUIRE(l->array_vals.size() == 3);
                required_if(CAN_MAKE(Literal*, v1)_FROM(l->array_vals[0])) {
                    REQUIRE(v1->literal_type == LiteralType::LITERAL_DOUBLE);
                    REQUIRE(v1->double_val == 1);
                }
                required_if(CAN_MAKE(Literal*, v2)_FROM(l->array_vals[1])) {
                    REQUIRE(v2->literal_type == LiteralType::LITERAL_DOUBLE);
                    REQUIRE(v2->double_val == 2);
                }
                required_if(CAN_MAKE(Literal*, v3)_FROM(l->array_vals[2])) {
                    REQUIRE(v3->literal_type == LiteralType::LITERAL_DOUBLE);
                    REQUIRE(v3->double_val == 3);
                }
            }
        }
    }

    SECTION("Error - hanging =") {
        REQUIRE_THROWS_WITH(getStatements("a x = ;"), "Expected primary but instead found: \";\", at line 1 and column 7, this token has type SEMI");
    }

    SECTION("Error - missing variable name") {
        REQUIRE_THROWS_WITH(getStatements("a = b"), "Expected variable name but instead found: \"=\", at line 1 and column 3, this token has type EQUALS");
    }

    SECTION("Error - missing semicolon") {
        REQUIRE_THROWS_WITH(getStatements("a x = b"), "Expect semi-colon after variable declaration but instead found: \"\", at line 1 and column 8, this token has type END");
    }
}

// Note that all of the above tests depend on the parser's
// ability to parse blocks, so we don't need to specifically test
// that.

TEST_CASE("Composite", "[parser]") {
    // A program is just a vector of statements, so we test a case of two such statements
    // and can reasonably assert that our parser will generalize to *n* statements
    // because the individual statement parsing tests above pass
    SECTION("Simple program") {
        auto statements = getStatements("p F;r T;");
        REQUIRE(statements.size() == 2);
        required_if(CAN_MAKE(Print*, p)_FROM(statements[0])) {
            required_if(CAN_MAKE(Literal*, l)_FROM(p->expr)) {
                REQUIRE(l->literal_type == LiteralType::LITERAL_BOOL);
                REQUIRE(l->bool_val == false);
            }
        }
        required_if(CAN_MAKE(Return*, r)_FROM(statements[1])) {
            required_if(CAN_MAKE(Literal*, l)_FROM(r->expr)) {
                REQUIRE(l->literal_type == LiteralType::LITERAL_BOOL);
                REQUIRE(l->bool_val == true);
            }
        }
    }
}

TEST_CASE("Add function", "[environment]"){
    Environment env;
    SECTION("Function"){
        Token token = {IDENTIFIER, "test", 1, 1}; 
        std::vector<Token> params; 
        std::vector<Stmt*> stmts; 
        FuncDecl* decl = new FuncDecl(token, params, stmts);
        env.add_func(decl->name.lexeme, decl);
        REQUIRE(env.func_symbol_table.find("test") != env.func_symbol_table.end());
    }
    SECTION("Operator"){
        Token name = {OPERATOR, "at", 1, 1}; 
        Token left = {NUMBER, "5", 5, 5}; 
        Token right = {NUMBER, "5", 5, 5}; 
        std::vector<Stmt*> stmts; 
        OpDecl* decl = new OpDecl(name, left, right, stmts);
        env.add_op(decl->name.lexeme, decl); 
        REQUIRE(env.op_symbol_table.find("at") != env.op_symbol_table.end()); 
    }
    SECTION("Variable"){
        Variable var = new Variable(); 
        env.add_var("test", var);
        REQUIRE(env.var_symbol_table.find("test") != env.var_symbol_table.end()); 
    }
}

//////////////////////////////////////////////////////////////////////////////
//                           Environment tests                              //
//////////////////////////////////////////////////////////////////////////////

std::string clean_output_string(std::string str) {
    std::vector<std::string> lines;
    std::string current;
    for(char c : str) {
        if(c == '\n') {
            if(!current.empty()) {
                lines.push_back(current);
            }
            current = "";
        } else if ((c == ' ' || c == '\t') && current.empty()) {
            // ignore
        } else {
            current += c;
        }
    }
    if(!current.empty()) {
        lines.push_back(current);
    }
    std::string result;
    for(auto line : lines) {
        result += line;
        result += '\n';
    }
    return result;
}

std::string clean_output_string(char* string) {
    std::string as_string = string;
    return clean_output_string(as_string);
}

#define REQUIRE_OUTPUT(prog, expected) REQUIRE(getOutput(prog) == clean_output_string(expected));

std::string getOutput(std::string program) {
    Lexer lex;
    auto lexed = lex.lex(program);
    Parser p (lexed);
    auto statements = p.parse();
    std::stringstream output_stream;
    Environment e (output_stream);
    for(auto stmt : statements) {
        e.execute_stmt(stmt);
    }
    return output_stream.str();
}

TEST_CASE("Printing simple expressions", "[environment]") {
    SECTION("string literal") {
        REQUIRE_OUTPUT("p \"hello\";", "\"hello\"");
    }

    SECTION("double") {
        REQUIRE_OUTPUT("p 2.5;", "2.5");
    }

    SECTION("bool") {
        REQUIRE_OUTPUT("p T;", "True");
        REQUIRE_OUTPUT("p F;", "False");
    }

    SECTION("1d array") {
        REQUIRE_OUTPUT("p [0];", "[0] sa [1]");
    }

    SECTION("nd array") {
        REQUIRE_OUTPUT("p [1, 2] sa [2, 2];", "[1, 2, 1, 2] sa [2, 2]");
    }
}

// For the following tests, we use print statements to get the results
// of expressions, meaning these tests are dependent on print functioning
// correctly. This is a reasonable assumption since it models the real-world
// usage of our environment, where users would be printing out values at
// the end of their program.

TEST_CASE("Math expression evaluation - literals only", "[environment]") {
    SECTION("Simple arithmetic") {
        REQUIRE_OUTPUT("p 2 + 3 * 5;", "17");
    }

    SECTION("Complex arithmetic") {
        REQUIRE_OUTPUT("p (2+3)^(1.5 * 2);", "125");
    }
}

TEST_CASE("Logical expression evaluation - literals only", "[environment]") {
    SECTION("Simple conditional") {
        REQUIRE_OUTPUT("p T O F;", "True");
    }

    SECTION("Complex conditional") {
        REQUIRE_OUTPUT("p !(!(T A F) O (!T O F));", "False");
    }
}

TEST_CASE("Variable declaration and usage", "[environment]") {
    SECTION("String declaration and usage") {
        auto program = R"V0G0N(
            a s_a = "cs";
            a s_b = s_a;
            p s_a;
            p s_b;
            s_a = "128";
            p s_b;
            p s_a;
        )V0G0N";
        auto result = R"V0G0N(
            "cs"
            "cs"
            "cs"
            "128"
        )V0G0N";
        REQUIRE_OUTPUT(program, result);
    }

    SECTION("Double declaration and usage") {
        auto program = R"V0G0N(
            a var = 1.5;
            a var_b = 4;
            a var_c = var_b ^ 2 * var;
            p var_c;
        )V0G0N";
        REQUIRE_OUTPUT(program, "24");
    }

    SECTION("1d array declaration and usage") {
        auto program = R"V0G0N(
            a arr = [0] sa [4];
            arr = arr + 2;
            p arr;
        )V0G0N";
        REQUIRE_OUTPUT(program, "[2, 2, 2, 2] sa [4]");
    }

    SECTION("nd array declaration and usage") {
        auto program = R"V0G0N(
            a arr = [0] sa [2, 2];
            arr = arr + 2;
            arr = arr * 3;
            p arr;
        )V0G0N";
        REQUIRE_OUTPUT(program, "[6, 6, 6, 6] sa [2, 2]");
    }

    SECTION("bool declaration and usage") {
        auto program = R"V0G0N(
            a boolean = T;
            a boolean_ = !boolean;
            p boolean;
            p boolean_;
        )V0G0N";
        auto output = R"V0G0N(
            True
            False
        )V0G0N";
        REQUIRE_OUTPUT(program, output);
    }
}

TEST_CASE("Function declaration and usage", "[environment]") {
    SECTION("Function with no parameters") {
        auto program = R"V0G0N(
            f func() {
                r 2 + 2;
            }

            a var = func() * 3 + 1;
            p var;
        )V0G0N";
        REQUIRE_OUTPUT(program, "13");
    }

    SECTION("Function with parameters") {
        auto program = R"V0G0N(
            f func(param_a, param_b, param_c) {
                r param_a + param_b + (param_c * 2);
            }

            p func(1, 2, 3);
            p func(2, 3, 4);
        )V0G0N";
        auto output= R"V0G0N(
            9
            13
        )V0G0N";
        REQUIRE_OUTPUT(program, output);
    }

    SECTION("Function called from other function") {
        auto program = R"V0G0N(
            f dim(list) {
                r (s (s list))[0];
            }

            f funky_multiply(list) {
                v dim(list) == 1;
                a len = (s list)[0];
                a j = 0;
                w (j < len) {
                    list[j] = len * list[j];
                    j = j + 1;
                }
                r list;
            }

            a arr = [1, 2, 3];
            p funky_multiply(arr);
        )V0G0N";
        REQUIRE_OUTPUT(program, "[3, 6, 9] sa [3]");
    }
}

TEST_CASE("Operator declaration and usage", "[environment]") {
    SECTION("Single custom operator") {
        auto program = R"V0G0N(
            o double_subtract(left, right) {
                r left - 2*right;
            }
            
            p 10 double_subtract 5;
        )V0G0N";
        REQUIRE_OUTPUT(program, "0");
    }

    SECTION("Custom operator used inside other custom operator") {
        auto program = R"V0G0N(
            o mult(left, right) {
                r left * -right;
            }

            o other(left, right) {
                r (left mult right) mult right;
            }

            p 5 other 10;
        )V0G0N";
        REQUIRE_OUTPUT(program, "500");
    }
}

TEST_CASE("While usage", "[environment]") {
    // There's really only one unique test we can do here,
    // any other tests would be isomorphic
    SECTION("While loop") {
        auto program = R"V0G0N(
            f inc(amount) {
                a j = 0;
                w (j < amount) {
                    j = j + 1;
                }
                r j;
            }
            p inc(10);
        )V0G0N";
        REQUIRE_OUTPUT(program, "10");
    }
}

TEST_CASE("If usage", "[environment]") {
    // Same as a while loop, we don't gain anything by testing
    // multiple different if statements since it just boils
    // down to condition evaluation which has already been tested
    SECTION("If statement") {
        auto program = R"V0G0N(
            a var = 10;
            i (var > 10) {
                p "Hello";
            }
            i (var <= 10) {
                p "World";
            }
        )V0G0N";
        REQUIRE_OUTPUT(program, "\"World\"");
    }
}

TEST_CASE("Assert usage", "[environment]") {
    // Same as an if, only need to do two tests here
    SECTION("Assert statement passes") {
        auto program = R"V0G0N(
            v 1 == 1;
            # If we get to this point the assert passed!
            p 1;
        )V0G0N";
        REQUIRE_OUTPUT(program, "1");
    }
    SECTION("Assert statement fails") {
        auto program = R"V0G0N(
            v 2 == 1;
        )V0G0N";
        REQUIRE_THROWS_WITH(getOutput(program), "Runtime error: Assert failed, occurred at line 1 at column 20");
    }
}

// Note that we do not test matrix multiplication above. This is because we use BLAS for
// matrix multiplication, and it would not make sense to test something that's already
// been tested.

TEST_CASE("Error tests", "[environment]") {
    SECTION("If statement without boolean condition") {
        REQUIRE_THROWS_WITH(getOutput("i (3) {}"), "Runtime error: If statement expected a boolean condition, occurred at line 0 at column 0");
    }

    SECTION("While statement without boolean condition") {
        REQUIRE_THROWS_WITH(getOutput("w (34) {}"), "Runtime error: While statement expected a boolean condition, occurred at line 0 at column 0");
    }

    SECTION("Assert statement without boolean condition") {
        REQUIRE_THROWS_WITH(getOutput("v 345;"), "Runtime error: Assert statement expected a boolean condition, occurred at line 0 at column 2");
    }

    SECTION("Array access on non-array") {
        auto program = R"V0G0N(
            a mat = 3;
            p mat[1];
        )V0G0N";
        REQUIRE_THROWS_WITH(getOutput(program), "Runtime error: Identifier in array access isn't an ndarray, occurred at line 2 at column 18");
    }

    SECTION("Incorrect array access dimensions") {
        auto program = R"V0G0N(
            a mat = [0, 1];
            p mat[0, 0];
        )V0G0N";
        REQUIRE_THROWS_WITH(getOutput(program), "Runtime error: Number of dimensions in array element access differs from number of dimensions in array, occurred at line 2 at column 18");
    }

    SECTION("Nonnumeric array access index") {
        auto program = R"V0G0N(
            a mat = [0, 1];
            p mat["hi"];
        )V0G0N";
        REQUIRE_THROWS_WITH(getOutput(program), "Runtime error: An expression used in array indexing is not a number, occurred at line 2 at column 18");
    }

    SECTION("Array access index too large") {
        auto program = R"V0G0N(
            a mat = [0, 1];
            p mat[2];
        )V0G0N";
        REQUIRE_THROWS_WITH(getOutput(program), "Runtime error: An expression used in array indexing is larger than a dimension of the ndarray, occurred at line 2 at column 18");
    }

    SECTION("Assignment to variable that does not exist") {
        REQUIRE_THROWS_WITH(getOutput("mat = 3;"), "Runtime error: Identifier doesn't correspond to a declared variable name, occurred at line 0 at column 0");
    }

    SECTION("Assign non-number to an array item") {
        auto program = R"V0G0N(
            a mat = [0, 1];
            mat[0] = "hi";
        )V0G0N";
        REQUIRE_THROWS_WITH(getOutput(program), "Runtime error: Can't assign a non-number to an entry in an array, occurred at line 2 at column 13");
    }

    SECTION("Using custom operator that does not exist") {
        REQUIRE_THROWS_WITH(getOutput("p T xor F;"), "Runtime error: Identifier doesn't correspond to a defined operator name, occurred at line 0 at column 4");
    }

    SECTION("Comparison with different types") {
        REQUIRE_THROWS_WITH(getOutput("p 2 <= \"a\";"), "Runtime error: Left and right expressions differ in type, occurred at line 0 at column 4");
    }

    SECTION("Matrix multiplication of non-matrix") {
        auto program = R"V0G0N(
            a mat = 3;
            p mat @ mat;
        )V0G0N";
        REQUIRE_THROWS_WITH(getOutput(program), "Runtime error: Left expression isn't an ndarray, occurred at line 2 at column 19");
    }

    SECTION("sa on non-matrix") {
        auto program = R"V0G0N(
            a non = "hello";
            p s non;
        )V0G0N";
        REQUIRE_THROWS_WITH(getOutput(program), "Runtime error: Expression evaluates to a non-ndarray, occurred at line 2 at column 15");
    }

    SECTION("Call to function that does not exist") {
        REQUIRE_THROWS_WITH(getOutput("p dne();"), "Runtime error: Identifier doesn't correspond to a defined function name, occurred at line 0 at column 2");
    }

    SECTION("Call to function with incorrect number of arguments") {
        auto program = R"V0G0N(
            f func(arg) {
                r arg;
            }
            p func(1, 2);
        )V0G0N";
        REQUIRE_THROWS_WITH(getOutput(program), "Runtime error: Function called with different number of args than defined with, occurred at line 4 at column 19");
    }

    SECTION("Creating array with non-doubles") {
        REQUIRE_THROWS_WITH(getOutput("a mat = [1, T];"), "Runtime error: Expression in array literal evaluates to a non-number, occurred at line 0 at column 8");
    }

    SECTION("Unary ! without bool") {
        REQUIRE_THROWS_WITH(getOutput("p !3;"), "Runtime error: Expression evaluates to a non-bool, occurred at line 0 at column 2");
    }

    SECTION("Unary - without number") {
        REQUIRE_THROWS_WITH(getOutput("p -T;"), "Runtime error: Expression evaluates to a non-number, occurred at line 0 at column 2");
    }

    SECTION("Unary s without array") {
        REQUIRE_THROWS_WITH(getOutput("p s T;"), "Runtime error: Expression evaluates to a non-ndarray, occurred at line 0 at column 2");
    }

    SECTION("Function call with variables that do not exist") {
        auto program = R"V0G0N(
            f func(arg) {
                r arg;
            }
            p func(arg);
        )V0G0N";
        REQUIRE_THROWS_WITH(getOutput(program), "Runtime error: Identifier doesn't correspond to a declared variable name, occurred at line 4 at column 20");
    }
}

TEST_CASE("Complete program evaluation", "[environment]") {
    SECTION("Program evaluation") {
        auto program = R"V0G0N(
            # This is a comment
            o x (b, c) {
                p "in operator x";
                r b ^ (c*2);
            }

            o z (b, c) {
                p "in operator z";
                r (s b) + (s c);
            }

            f y (b, c, d) {
                p "in function y";
                a m = 0;
                a mat = [0] sa [2, 2];
                w (m < d) {
                    p "in while loop";
                    mat = mat x mat;
                    mat = mat @ mat;
                    m = m + 1;
                }
                i (mat[0, 0] < 10^3) {
                    p mat[0, 0];
                }
                i (mat[0, 0] != 1 O mat[1, 1] != 2) {
                    r T;
                }
                r F;
            }

            p "== First call ==";
            p y(1,2,3);
            p "== Second call ==";
            p "Hello";
            p "== Third call ==";
            p N;
            p "== Fourth call ==";
            p y(2,3,4);
        )V0G0N";
        auto output = R"V0G0N(
            "== First call =="
            "in function y"
            "in while loop"
            "in operator x"
            "in while loop"
            "in operator x"
            "in while loop"
            "in operator x"
            True
            "== Second call =="
            "Hello"
            "== Third call =="
            Nil
            "== Fourth call =="
            "in function y"
            "in while loop"
            "in operator x"
            "in while loop"
            "in operator x"
            "in while loop"
            "in operator x"
            "in while loop"
            "in operator x"
            True
        )V0G0N";
        REQUIRE_OUTPUT(program, output);
    };
}
