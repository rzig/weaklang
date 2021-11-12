#include "stmt.h"

Stmt::~Stmt();

Block::Block(std::vector<Stmt*> stmts) {}

ExprStmt::ExprStmt(Expr* expr) {}

FuncDecl::FuncDecl(Token name, std::vector<Token> params, std::vector<Stmt*> stmts) {}

If::If(Expr* cond, Stmt* pos_branch, Stmt* neg_branch) {}

OpDecl::OpDecl(Token name, Token left, Token right, std::vector<Stmt*> stmts) {}

Print::Print(Expr* expr) {}

Return::Return(Token return_keyword, Expr* expr) {}

VarDecl::VarDecl(Token name, Expr* expr) {}

While::While(Expr* cond, Stmt* body) {}

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
