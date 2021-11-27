#include "stmt.hpp"

Stmt::~Stmt() {}

size_t Stmt::statement_counter = 0;

template <typename T>
std::pair<std::string, std::string> Stmt::make_string(std::string label, std::vector<T*> children) {
    std::string id = "statement";
    id += std::to_string(statement_counter++);
    std::vector<std::pair<std::string,std::string>> children_strs;
    for(auto* child : children) {
        children_strs.push_back(child->to_string());
    }
    // Conform to DOT file syntax
    for(size_t i = 0; i < label.size(); ++i) {
        if (label[i] == '"') {
            label[i] = '\'';
        }
    }
    std::string g;
    g += id;
    g += "[";
    g += "label=\"";
    g += label;
    g += "\", shape=\"circle\"]\n";
    for(auto x : children_strs) {
        g += x.second;
        g += '\n';
    }
    for(auto x : children_strs) {
        g += id;
        g += "->";
        g += x.first;
        g += "\n";
    }
    return std::make_pair(id, g);
}

template <typename T>
std::pair<std::string, std::string> Stmt::make_string(std::string label, std::initializer_list<T*> children) {
    std::vector<Expr*> child_vec;
    for(auto* e : children) {
        child_vec.push_back(e);
    }
    return make_string(label, child_vec);
}

template <typename T>
std::pair<std::string, std::string> Stmt::make_string(std::string label, T* child) {
    std::vector<Expr*> c;
    c.push_back(child);
    return make_string(label, c);
}

ExprStmt::ExprStmt(Expr* expr): expr(expr) {}

std::pair<std::string, std::string> ExprStmt::to_string() {
    return make_string("Expression statement", expr);
}

FuncDecl::FuncDecl(Token name, std::vector<Token> params, std::vector<Stmt*> stmts): name(name), params(params), stmts(stmts) {}

std::pair<std::string, std::string> FuncDecl::to_string() {
    return make_string("Declare Function " + name.lexeme, stmts);
}

If::If(Token keyword, Expr* cond, std::vector<Stmt*> stmts): keyword(keyword), cond(cond), stmts(stmts) {}

std::pair<std::string, std::string> If::to_string() {
    return make_string("If Statement ", stmts);
}

OpDecl::OpDecl(Token name, Token left, Token right, std::vector<Stmt*> stmts): name(name), left(left), right(right), stmts(stmts) {}

std::pair<std::string, std::string> OpDecl::to_string() {
    return make_string("Declare Operator " + name.lexeme, stmts);
}

Print::Print(Token print_keyword, Expr* expr): print_keyword(print_keyword), expr(expr) {}

std::pair<std::string, std::string> Print::to_string() {
    return make_string("Print Statement", expr);
}

Return::Return(Token return_keyword, Expr* expr): return_keyword(return_keyword), expr(expr) {}

std::pair<std::string, std::string> Return::to_string() {
    return make_string("Return Statement", expr);
}

VarDecl::VarDecl(Token name, Expr* expr): name(name), expr(expr) {}

std::pair<std::string, std::string> VarDecl::to_string() {
    return make_string("Declare variable " + name.lexeme, expr);
}

While::While(Token keyword, Expr* cond, std::vector<Stmt*> stmts): keyword(keyword), cond(cond), stmts(stmts) {}

std::pair<std::string, std::string> While::to_string() {
    return make_string("While Statement", stmts);
}

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
