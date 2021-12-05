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

#ifndef STMT_H_
#define STMT_H_

#include <vector>

#include "token.hpp"
#include "expr.hpp"

class Stmt {
public:
    virtual ~Stmt();
    virtual std::pair<std::string, std::string> to_string() = 0;
    static size_t statement_counter;
    template <typename T>
    static std::pair<std::string, std::string> make_string(std::string label, T* child);
    template <typename T>
    static std::pair<std::string, std::string> make_string(std::string label, std::initializer_list<T*> children);
    template <typename T>
    static std::pair<std::string, std::string> make_string(std::string label, std::vector<T*> exprs);
};

class ExprStmt : public Stmt {
public:
    ExprStmt(Expr* expr);
    ~ExprStmt();
    std::pair<std::string, std::string> to_string();
    Expr* expr;
};

class FuncDecl : public Stmt {
public:
    FuncDecl(Token name, std::vector<Token> params, std::vector<Stmt*> stmts);
    ~FuncDecl();
    std::pair<std::string, std::string> to_string();
    Token name;
    std::vector<Token> params;
    std::vector<Stmt*> stmts;
};

class If : public Stmt {
public:
    If(Token keyword, Expr* cond, std::vector<Stmt*> stmts);
    ~If();
    std::pair<std::string, std::string> to_string();
    Token keyword;
    Expr* cond;
    std::vector<Stmt*> stmts;
};

class OpDecl : public Stmt {
public:
    OpDecl(Token name, Token left, Token right, std::vector<Stmt*> stmts);
    ~OpDecl();
    std::pair<std::string, std::string> to_string();
    Token name;
    Token left;
    Token right;
    std::vector<Stmt*> stmts;
};

class Print : public Stmt {
public:
    Print(Token print_keyword, Expr* expr);
    ~Print();
    std::pair<std::string, std::string> to_string();
    Token print_keyword;
    Expr* expr;
};

class Return : public Stmt {
public:
    Return(Token return_keyword, Expr* expr);
    ~Return();
    std::pair<std::string, std::string> to_string();
    Token return_keyword;
    Expr* expr;
};

class VarDecl : public Stmt {
public:
    VarDecl(Token name, Expr* expr);
    ~VarDecl();
    std::pair<std::string, std::string> to_string();
    Token name;
    Expr* expr;
};

class While : public Stmt {
public:
    While(Token keyword, Expr* cond, std::vector<Stmt*> stmts);
    ~While();
    std::pair<std::string, std::string> to_string();
    Token keyword;
    Expr* cond;
    std::vector<Stmt*> stmts;
};

class Assert : public Stmt {
    public:
    Assert(Token keyword, Expr* cond);
    ~Assert(); 
    std::pair<std::string, std::string> to_string(); 
    Token keyword; 
    Expr* cond; 
};

#endif // STMT_H_
