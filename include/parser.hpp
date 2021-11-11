#ifndef PARSER_H_
#define PARSER_H_

#include "token.hpp"
#include "stmt.hpp"
#include "expr.hpp"

#include <vector>

class Parser {
public:
    Parser(std::vector<Token> input);
    Stmt program();
private:
    std::vector<Token> tokens;

    Stmt declaration();
    Stmt funDeclaration();
    Stmt opDeclaration();
    Stmt varDeclaration();
    Stmt statement();
    Stmt block();

    Stmt exprStatement();
    Stmt ifStatement();
    Stmt printStatement();
    Stmt whileStatement();
    Stmt returnStatement();

    Expr expression(); 
    Expr assignment();
    Expr logicOr();
    Expr logicAnd();
    Expr equality();
    Expr comparison();
    Expr term();
    Expr factor();
    Expr unary();
    Expr primary();
};

#endif // PARSER_H_
