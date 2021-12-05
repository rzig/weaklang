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

#ifndef PARSER_H_
#define PARSER_H_

#include "token.hpp"
#include "stmt.hpp"
#include "expr.hpp"

#include <stdexcept>
#include <vector>

#define MAX_ARGS 64

class Parser {
public:
    Parser(std::vector<Token> input);
    std::vector<Stmt*> parse();
    std::string as_dot();
private:
    std::vector<Token> tokens;
    std::vector<Stmt*> decls;
    size_t cur_index = 0;

    bool match(TokenType type);
    bool match(TokenType type1, TokenType type2);
    bool match(std::initializer_list<TokenType> types);
    bool currently_at(TokenType type);
    bool currently_at(std::initializer_list<TokenType> types);
    Token consume(TokenType type, std::string message);
    Token consume(std::initializer_list<TokenType> types, std::string message);
    std::string create_error(Token token, std::string message);

    Stmt* declaration();
    Stmt* funDeclaration();
    Stmt* opDeclaration();
    Stmt* varDeclaration();
    Stmt* statement();
    std::vector<Stmt*> block();

    Stmt* exprStatement();
    Stmt* ifStatement();
    Stmt* printStatement();
    Stmt* whileStatement();
    Stmt* returnStatement();
    Stmt* assertStatement(); 

    Expr* expression();
    Expr* function();
    Expr* operation();
    Expr* assignment();
    Expr* logicOr();
    Expr* logicAnd();
    Expr* equality();
    Expr* comparison();
    Expr* term();
    Expr* factor();
    Expr* unary();
    Expr* arrAccess();
    Expr* primary();
};

#endif // PARSER_H_
