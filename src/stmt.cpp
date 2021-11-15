#include "stmt.hpp"

Stmt::~Stmt() {}

ExprStmt::ExprStmt(Expr* expr): expr(expr) {}

FuncDecl::FuncDecl(Token name, std::vector<Token> params, std::vector<Stmt*> stmts): name(name), params(params), stmts(stmts) {}

If::If(Expr* cond, std::vector<Stmt*> stmts): cond(cond), stmts(stmts) {}

OpDecl::OpDecl(Token name, Token left, Token right, std::vector<Stmt*> stmts): name(name), left(left), right(right), stmts(stmts) {}

Print::Print(Expr* expr): expr(expr) {}

Return::Return(Expr* expr): expr(expr) {}

VarDecl::VarDecl(Token name, Expr* expr): name(name), expr(expr) {}

While::While(Expr* cond, std::vector<Stmt*> stmts): cond(cond), stmts(stmts) {}

ExprStmt::~ExprStmt() {
    delete expr;
}

FuncDecl::~FuncDecl() {
    for (auto stmt : stmts) delete stmt;
}

If::~If() {
    delete cond;
    for (auto stmt : stmts) delete stmt;
}

OpDecl::~OpDecl() {
    for (auto stmt : stmts) delete stmt;
}

Print::~Print() {
    delete expr;
}

Return::~Return() {
    delete expr;
}

VarDecl::~VarDecl() {
    delete expr;
}

While::~While() {
    delete cond;
    for(auto stmt : stmts) delete stmt;
}
