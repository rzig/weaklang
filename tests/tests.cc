
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
        lex.lex("ü");
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

#define REQUIRE_ABILITY(x) if(x) {REQUIRE(true);} else {REQUIRE(false);}
#define TO_MAKE(type, variable) CAN_MAKE(type, variable)
#define required_if(condition) REQUIRE_ABILITY(condition); if(condition)

using Svec = std::vector<Stmt*>;

Svec getStatements(std::string program) {
    Lexer lex;
    Parser p {lex.lex(program)};
    return p.parse();
}

TEST_CASE("Assignment", "[parser]"){
    SECTION("to literal") {
        Svec statements = getStatements("a x = 5;");
        required_if(CAN_MAKE(VarDecl*, v)_FROM(statements[0])) {
            REQUIRE(v->name.lexeme == "x");
            required_if(CAN_MAKE(Literal*, l)_FROM(v->expr)) {
                REQUIRE(l->literal_type == LiteralType::LITERAL_DOUBLE);
                REQUIRE(l->double_val == 5);
            }
        }
    }

    SECTION("to function") {
        Svec statements = getStatements("a var = func(b, c);");
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
        Svec statements = getStatements("a array = [1,2,3];");
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
}

TEST_CASE("Operation", "[parser]") {
    SECTION("with single or") {
        Svec statements = getStatements("var O T;");
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
        Svec statements = getStatements("func() A arr[1];");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(e->expr)) {
                REQUIRE(b->op.type == AND);
                required_if(CAN_MAKE(Func*, f)_FROM(b->left)) {
                    REQUIRE(f->func.lexeme == "func");
                    REQUIRE(f->args.size() == 0);
                }
                required_if(CAN_MAKE(ArrAccess*, a)_FROM(b->right)) {
                    REQUIRE(a->id.lexeme == "arr");
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
        Svec statements = getStatements("T O F A T;");
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
}

TEST_CASE("Equality", "[parser]") {
    SECTION("with two literals and ==") {
        Svec statements = getStatements("T == 1;");
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
        Svec statements = getStatements("v(b) == b;");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(e->expr)) {
                required_if(CAN_MAKE(Func*, f)_FROM(b->left)) {
                    REQUIRE(f->func.lexeme == "v");
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
        Svec statements = getStatements("v != 10;");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(e->expr)) {
                required_if(CAN_MAKE(Var*, v)_FROM(b->left)) {
                    REQUIRE(v->name.lexeme == "v");
                }
                required_if(CAN_MAKE(Literal*, l)_FROM(b->right)) {
                    REQUIRE(l->literal_type == LiteralType::LITERAL_DOUBLE);
                    REQUIRE(l->double_val == 10);
                }
                REQUIRE(b->op.type == EXCLA_EQUALS);
            }
        }
    }
}

TEST_CASE("Comparison", "[parser]") {
    SECTION("literal > literal") {
        Svec statements = getStatements("2 > 3;");
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
        Svec statements = getStatements("var >= func(b,2);");
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
        Svec statements = getStatements("v() <= 3;");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(e->expr)) {
                required_if(CAN_MAKE(Func*, f)_FROM(b->left)) {
                    REQUIRE(f->func.lexeme == "v");
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
        Svec statements = getStatements("arr[1] < -x;");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(e->expr)) {
                required_if(CAN_MAKE(ArrAccess*, a)_FROM(b->left)) {
                    REQUIRE(a->id.lexeme == "arr");
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
}

TEST_CASE("Terms and factors", "[parser]") {
    SECTION("multiple addition and subtraction") {
        Svec statements = getStatements("d + b - c + 3;");
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
        Svec statements = getStatements("d * b - c / 3;");
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
        Svec statements = getStatements("(b@c) + (c/d) * (d^e) * s arr;");
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
}

TEST_CASE("Unary operations", "[parser]") {
    SECTION("!") {
        Svec statements = getStatements("!var;");
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
        Svec statements = getStatements("-x;");
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
        Svec statements = getStatements("s (c + d);");
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
}

TEST_CASE("Array access", "[parser]") {
    SECTION("1D") {
        Svec statements = getStatements("array[1];");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(ArrAccess*, a)_FROM(e->expr)) {
                REQUIRE(a->id.lexeme == "array");
                REQUIRE(a->idx.size() == 1);
                required_if(CAN_MAKE(Literal*, one)_FROM(a->idx[0])) {
                    REQUIRE(one->literal_type == LiteralType::LITERAL_DOUBLE);
                    REQUIRE(one->double_val == 1);
                }
            }
        }
    }

    SECTION("2D") {
        Svec statements = getStatements("mat[1, b];");
        required_if(CAN_MAKE(ExprStmt*, e)_FROM(statements[0])) {
            required_if(CAN_MAKE(ArrAccess*, a)_FROM(e->expr)) {
                REQUIRE(a->id.lexeme == "mat");
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
}

TEST_CASE("While statements", "[parser]") {
    SECTION("with simple expression") {
        Svec statements = getStatements("w (var) {var = var O T;}");
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
        Svec statements = getStatements("w (var A !func(var)) {}");
        required_if(CAN_MAKE(While*, w)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, b)_FROM(w->cond)) {
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
}

TEST_CASE("Return statement", "[parser]") {
    SECTION("with no expression") {
        Svec statements = getStatements("r;");
        required_if(CAN_MAKE(Return*, r)_FROM(statements[0])) {
            REQUIRE_ABILITY(TO_MAKE(Nil*, n)_FROM(r->expr));
        }
    }

    SECTION("with expression") {
        Svec statements = getStatements("r var + 2;");
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
}

TEST_CASE("Print statement", "[parser]") {
    SECTION("with simple expression") {
        Svec statements = getStatements("p it;");
        required_if(CAN_MAKE(Print*, p)_FROM(statements[0])) {
            required_if(CAN_MAKE(Var*, it)_FROM(p->expr)) {
                REQUIRE(it->name.lexeme == "it");
            }
        }
    }

    SECTION("with complex expression") {
        Svec statements = getStatements("p v() + b@b;");
        required_if(CAN_MAKE(Print*, p)_FROM(statements[0])) {
            required_if(CAN_MAKE(Binary*, parent)_FROM(p->expr)) {
                required_if(CAN_MAKE(Func*, v)_FROM(parent->left)) {
                    REQUIRE(v->func.lexeme == "v");
                    REQUIRE(v->args.size() == 0);
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
}

// TEST_CASE("If statement", "[parser]") {
//     SECTION("with simple expression") {

//     }

//     SECTION("with complex expression") {

//     }
// }

// Note that all of the above tests depend on the parser's
// ability to parse blocks, so we don't need to specifically test
// that.