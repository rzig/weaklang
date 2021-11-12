#ifndef STMT_H_
#define STMT_H_

#include <vector>

#include "token.hpp"
#include "expr.hpp"

class Stmt {
public:
    virtual ~Stmt();
};

class Block : Stmt {
public:
    Block(std::vector<Stmt*> stmts);
    ~Block();
private:
    std::vector<Stmt*> stmts;
};

class ExprStmt : Stmt {
public:
    ExprStmt(Expr* expr);
    ~ExprStmt();
private:
    Expr* expr;
};

class FuncDecl : Stmt {
public:
    FuncDecl(Token name, std::vector<Token> params, std::vector<Stmt*> stmts);
    ~FuncDecl();
private:
    Token name;
    std::vector<Token> params;
    std::vector<Stmt*> stmts;
};

class If : Stmt {
public:
    If(Expr* cond, Stmt* pos_branch, Stmt* neg_branch);
    ~If();
private:
    Expr* cond;
    Stmt* pos_branch;
    Stmt* neg_branch;
};

class OpDecl : Stmt {
public:
    OpDecl(Token name, Token left, Token right, std::vector<Stmt*> stmts);
    ~OpDecl();
private:
    Token name;
    Token left;
    Token right;
    std::vector<Stmt*> stmts;
};

class Print : Stmt {
public:
    Print(Expr* expr);
    ~Print();
private:
    Expr* expr;
};

class Return : Stmt {
public:
    Return(Token return_keyword, Expr* expr);
    ~Return();
private:
    Token return_keryword;
    Expr* expr;
};

class VarDecl : Stmt {
public:
    VarDecl(Token name, Expr* expr);
    ~VarDecl();
private:
    Token name;
    Expr* expr;
};

class While : Stmt {
public:
    While(Expr* cond, Stmt* body);
    ~While();
private:
    Expr* cond;
    Stmt* body;
};

#endif // STMT_H_
