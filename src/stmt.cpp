#include "stmt.h"

Stmt::~Stmt();

Block::Block(std::vector<Stmt*> stmts): stmts(stmts) {}

ExprStmt::ExprStmt(Expr* expr): expr(expr) {}

FuncDecl::FuncDecl(Token name, std::vector<Token> params, std::vector<Stmt*> stmts): name(name), params(params), stmts(stmts) {}

If::If(Expr* cond, Stmt* pos_branch, Stmt* neg_branch): cond(cond), pos_branch(pos_branch), neg_branch(neg_branch) {}

OpDecl::OpDecl(Token name, Token left, Token right, std::vector<Stmt*> stmts): name(name), left(left), right(right), stmts(stmts) {}

Print::Print(Expr* expr): expr(expr) {}

Return::Return(Token return_keyword, Expr* expr): return_keyword(return_keyword), expr(expr) {}

VarDecl::VarDecl(Token name, Expr* expr): name(name), expr(expr) {}

While::While(Expr* cond, Stmt* body): cond(cond), body(body) {}

Block::~Block() {
    for (auto stmt : stmts) delete stmt;
}

ExprStmt::~ExprStmt() {
    delete expr;
}

FuncDecl::~FuncDecl() {
    for (auto stmt : stmts) delete stmt;
}

If::~If() {
    delete cond;
    delete pos_branch;
    delete neg_branch;
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
    delete body;
}
