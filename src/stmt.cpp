#include "stmt.hpp"

Stmt::~Stmt() {}

size_t Stmt::statement_counter = 0;

std::pair<std::string, std::string> Stmt::ToString() {
    std::string a = "";
    std::string b = "";
    return std::make_pair(a, b);
}

ExprStmt::ExprStmt(Expr* expr): expr(expr) {}

std::pair<std::string, std::string> ExprStmt::ToString() {
    std::string label = "Expression Statement";
    std::string id = "statement";
    id += std::to_string(statement_counter++);
    std::pair<std::string,std::string> children = expr->ToString();
    std::string g = "";
    g += id;
    g += "[";
    g += "label=\"";
    g += label;
    g += "\", shape=\"square\"]\n";
    g += children.second;
    g += "\n";
    g += id;
    g += "->";
    g += children.first;
    g += "\n";
    return std::make_pair(id, g);
}

FuncDecl::FuncDecl(Token name, std::vector<Token> params, std::vector<Stmt*> stmts): name(name), params(params), stmts(stmts) {}

std::pair<std::string, std::string> FuncDecl::ToString() {
    std::string label = "Declare Function ";
    label += name.lexeme;
    std::string id = "statement";
    id += std::to_string(statement_counter++);
    std::vector<std::pair<std::string,std::string>> children_strs;
    for(Stmt* stmt : stmts) {
        children_strs.push_back(stmt->ToString());
    }
    std::string g = "";
    g += id;
    g += "[";
    g += "label=\"";
    g += label;
    g += "\", shape=\"square\"]\n";
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

If::If(Expr* cond, std::vector<Stmt*> stmts): cond(cond), stmts(stmts) {}

std::pair<std::string, std::string> If::ToString() {
    std::string label = "If Statement ";
    std::string id = "statement";
    id += std::to_string(statement_counter++);
    std::vector<std::pair<std::string,std::string>> children_strs;
    for(Stmt* stmt : stmts) {
        children_strs.push_back(stmt->ToString());
    }
    std::pair<std::string, std::string> ex = cond->ToString();
    std::string g = "";
    g += id;
    g += "[";
    g += "label=\"";
    g += label;
    g += "\", shape=\"square\"]\n";
    for(auto x : children_strs) {
        g += x.second;
        g += '\n';
    }
    g += ex.second;
    g += '\n';
    for(auto x : children_strs) {
        g += id;
        g += "->";
        g += x.first;
        g += "\n";
    }
    g += id;
    g += "->";
    g += ex.first;
    g += "\n";
    return std::make_pair(id, g);
}

OpDecl::OpDecl(Token name, Token left, Token right, std::vector<Stmt*> stmts): name(name), left(left), right(right), stmts(stmts) {}

std::pair<std::string, std::string> OpDecl::ToString() {
    std::string label = "Declare Operator ";
    label += name.lexeme;
    std::string id = "statement";
    id += std::to_string(statement_counter++);
    std::vector<std::pair<std::string,std::string>> children_strs;
    for(Stmt* stmt : stmts) {
        children_strs.push_back(stmt->ToString());
    }
    std::string g = "";
    g += id;
    g += "[";
    g += "label=\"";
    g += label;
    g += "\", shape=\"square\"]\n";
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

Print::Print(Token print_keyword, Expr* expr): print_keyword(print_keyword), expr(expr) {}

std::pair<std::string, std::string> Print::ToString() {
    std::string label = "Print Statement";
    std::string id = "statement";
    id += std::to_string(statement_counter++);
    std::pair<std::string,std::string> children = expr->ToString();
    std::string g = "";
    g += id;
    g += "[";
    g += "label=\"";
    g += label;
    g += "\", shape=\"square\"]\n";
    g += children.second;
    g += "\n";
    g += id;
    g += "->";
    g += children.first;
    g += "\n";
    return std::make_pair(id, g);
}

Return::Return(Token return_keyword, Expr* expr): return_keyword(return_keyword), expr(expr) {}

std::pair<std::string, std::string> Return::ToString() {
    std::string label = "Return Statement";
    std::string id = "statement";
    id += std::to_string(statement_counter++);
    std::pair<std::string,std::string> children = expr->ToString();
    std::string g = "";
    g += id;
    g += "[";
    g += "label=\"";
    g += label;
    g += "\", shape=\"square\"]\n";
    g += children.second;
    g += "\n";
    g += id;
    g += "->";
    g += children.first;
    g += "\n";
    return std::make_pair(id, g);
}

VarDecl::VarDecl(Token name, Expr* expr): name(name), expr(expr) {}

std::pair<std::string, std::string> VarDecl::ToString() {
    std::string label = "Declare variable ";
    label += name.lexeme;
    std::string id = "statement";
    id += std::to_string(statement_counter++);
    std::pair<std::string,std::string> children = expr->ToString();
    std::string g = "";
    g += id;
    g += "[";
    g += "label=\"";
    g += label;
    g += "\", shape=\"square\"]\n";
    g += children.second;
    g += "\n";
    g += id;
    g += "->";
    g += children.first;
    g += "\n";
    return std::make_pair(id, g);
}

While::While(Expr* cond, std::vector<Stmt*> stmts): cond(cond), stmts(stmts) {}

std::pair<std::string, std::string> While::ToString() {
    std::string label = "If Statement ";
    std::string id = "statement";
    id += std::to_string(statement_counter++);
    std::vector<std::pair<std::string,std::string>> children_strs;
    for(Stmt* stmt : stmts) {
        children_strs.push_back(stmt->ToString());
    }
    std::pair<std::string, std::string> ex = cond->ToString();
    std::string g = "";
    g += id;
    g += "[";
    g += "label=\"";
    g += label;
    g += "\", shape=\"square\"]\n";
    for(auto x : children_strs) {
        g += x.second;
        g += '\n';
    }
    g += ex.second;
    g += '\n';
    for(auto x : children_strs) {
        g += id;
        g += "->";
        g += x.first;
        g += "\n";
    }
    g += id;
    g += "->";
    g += ex.first;
    g += "\n";
    return std::make_pair(id, g);
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
