#ifndef STMT_H_
#define STMT_H_

#include <vector>

#include "token.hpp"
#include "expr.hpp"

class Stmt {
public:
    virtual ~Stmt();
    virtual std::pair<std::string, std::string> ToString();
    static size_t statement_counter;
};

class ExprStmt : public Stmt {
public:
    ExprStmt(Expr* expr);
    ~ExprStmt();
    std::pair<std::string, std::string> ToString();
private:
    Expr* expr;
};

class FuncDecl : public Stmt {
public:
    FuncDecl(Token name, std::vector<Token> params, std::vector<Stmt*> stmts);
    ~FuncDecl();
    std::pair<std::string, std::string> ToString();
private:
    Token name;
    std::vector<Token> params;
    std::vector<Stmt*> stmts;
};

class If : public Stmt {
public:
    If(Expr* cond, std::vector<Stmt*> stmts);
    ~If();
    std::pair<std::string, std::string> ToString();
private:
    Expr* cond;
    std::vector<Stmt*> stmts;
};

class OpDecl : public Stmt {
public:
    OpDecl(Token name, Token left, Token right, std::vector<Stmt*> stmts);
    ~OpDecl();
    std::pair<std::string, std::string> ToString();
private:
    Token name;
    Token left;
    Token right;
    std::vector<Stmt*> stmts;
};

class Print : public Stmt {
public:
    Print(Token print_keyword, Expr* expr);
    ~Print();
    std::pair<std::string, std::string> ToString();
private:
    Token print_keyword;
    Expr* expr;
};

class Return : public Stmt {
public:
    Return(Token return_keyword, Expr* expr);
    ~Return();
    std::pair<std::string, std::string> ToString();
private:
    Token return_keyword;
    Expr* expr;
};

class VarDecl : public Stmt {
public:
    VarDecl(Token name, Expr* expr);
    ~VarDecl();
    std::pair<std::string, std::string> ToString();
private:
    Token name;
    Expr* expr;
};

class While : public Stmt {
public:
    While(Expr* cond, std::vector<Stmt*> stmts);
    ~While();
    std::pair<std::string, std::string> ToString();
private:
    Expr* cond;
    std::vector<Stmt*> stmts;
};

#endif // STMT_H_
