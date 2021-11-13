#ifndef STMT_H_
#define STMT_H_

#include <vector>

#include "token.hpp"
#include "expr.hpp"

class Stmt {
public:
    virtual ~Stmt();
};

class ExprStmt : public Stmt {
public:
    ExprStmt(Expr* expr);
    ~ExprStmt();
private:
    Expr* expr;
};

class FuncDecl : public Stmt {
public:
    FuncDecl(Token name, std::vector<Token> params, std::vector<Stmt*> stmts);
    ~FuncDecl();
private:
    Token name;
    std::vector<Token> params;
    std::vector<Stmt*> stmts;
};

class If : public Stmt {
public:
    If(Expr* cond, Stmt* pos_branch, Stmt* neg_branch);
    ~If();
private:
    Expr* cond;
    Stmt* pos_branch;
    Stmt* neg_branch;
};

class OpDecl : public Stmt {
public:
    OpDecl(Token name, Token left, Token right, std::vector<Stmt*> stmts);
    ~OpDecl();
private:
    Token name;
    Token left;
    Token right;
    std::vector<Stmt*> stmts;
};

class Print : public Stmt {
public:
    Print(Expr* expr);
    ~Print();
private:
    Expr* expr;
};

class Return : public Stmt {
public:
    Return(Token return_keyword, Expr* expr);
    ~Return();
private:
    Token return_keyword;
    Expr* expr;
};

class VarDecl : public Stmt {
public:
    VarDecl(Token name, Expr* expr);
    ~VarDecl();
private:
    Token name;
    Expr* expr;
};

class While : public Stmt {
public:
    While(Expr* cond, Stmt* body);
    ~While();
private:
    Expr* cond;
    Stmt* body;
};

#endif // STMT_H_
