#include "expr.hpp"
#include<string>

Expr::~Expr() {}

std::pair<std::string, std::string> Expr::ToString() {
    std::string a = "";
    std::string b = "";

    return std::make_pair(a,b);
}

size_t Expr::node_counter = 0;

Assign::Assign(Token name, Expr* value): name(name), value(value) {}

std::pair<std::string, std::string> Assign::ToString() {
    std::string label = "Assignment operator, lexeme: ";
    label += name.lexeme;
    std::string id = "assignment";
    id += std::to_string(node_counter++);
    std::pair<std::string,std::string> children = value->ToString();
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

Binary::Binary(Expr* left, Token op, Expr* right): left(left), op(op), right(right) {}

std::pair<std::string, std::string> Binary::ToString() {
    std::string label = "Binary operator";
    label += op.lexeme;
    std::string id = "assignment";
    id += std::to_string(node_counter++);
    std::pair<std::string,std::string> children_left = left->ToString();
    std::pair<std::string,std::string> children_right = right->ToString();
    std::string g = "";
    g += id;
    g += "[";
    g += "label=\"";
    g += label;
    g += "\", shape=\"square\"]\n";
    g += children_left.second;
    g += "\n";
    g += children_right.second;
    g += "\n";
    g += id;
    g += "->";
    g += children_left.first;
    g += "\n";
    g += id;
    g += "->";
    g += children_right.first;
    g += "\n";
    return std::make_pair(id, g);
}

Func::Func(Token func, Token paren, std::vector<Expr*> args): func(func), paren(paren), args(args) {}

std::pair<std::string, std::string> Func::ToString() {
    std::string label = "Function, lexeme: ";
    label += func.lexeme;
    std::string id = "assignment";
    id += std::to_string(node_counter++);
    std::vector<std::pair<std::string,std::string>> children_strs;
    for(Expr* arg : args) {
        children_strs.push_back(arg->ToString());
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

Group::Group(Expr* expr): expr(expr) {}

std::pair<std::string, std::string> Group::ToString() {
    std::string label = "Group";
    std::string id = "assignment";
    id += std::to_string(node_counter++);
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

Literal::Literal(std::string val): string_val(val) {}

Literal::Literal(double val): double_val(val) {}

Literal::Literal(bool val): bool_val(val) {}

Literal::Literal(std::vector<double> vals): array_vals(vals) {}

std::pair<std::string, std::string> Literal::ToString() {
    std::string label = "Literal ";
    if(not array_vals.empty()) {
        for(auto v : array_vals) {
            label += v;
            label += ", ";
        }
    } else if(not string_val.empty()) {
        label += string_val;
    } else if (not (double_val == 0)) {
        label += double_val;
    } else {
        label += bool_val;
    }
    std::string id = "assignment";
    id += std::to_string(node_counter++);
    std::string g = "";
    g += id;
    g += "[";
    g += "label=\"";
    g += label;
    g += "\", shape=\"square\"]\n";
    return std::make_pair(id, g);
}

Logical::Logical(Expr* left, Token op, Expr* right): left(left), op(op), right(right) {}

std::pair<std::string, std::string> Logical::ToString() {
    std::string label = "Logical operator ";
    label += op.lexeme;
    std::string id = "assignment";
    id += std::to_string(node_counter++);
    std::pair<std::string,std::string> children_left = left->ToString();
    std::pair<std::string,std::string> children_right = right->ToString();
    std::string g = "";
    g += id;
    g += "[";
    g += "label=\"";
    g += label;
    g += "\", shape=\"square\"]\n";
    g += children_left.second;
    g += "\n";
    g += children_right.second;
    g += "\n";
    g += id;
    g += "->";
    g += children_left.first;
    g += "\n";
    g += id;
    g += "->";
    g += children_right.first;
    g += "\n";
    return std::make_pair(id, g);
}

Nil::Nil() {}

std::pair<std::string, std::string> Nil::ToString() {
    std::string label = "NIL";
    std::string id = "assignment";
    id += std::to_string(node_counter++);
    std::string g = "";
    g += id;
    g += "[";
    g += "label=\"";
    g += label;
    g += "\", shape=\"square\"]\n";
    return std::make_pair(id, g);
}

Op::Op(Token op, Token paren, std::vector<Expr*> args): op(op), paren(paren), args(args) {}

std::pair<std::string, std::string> Op::ToString() {
    std::string label = "Option, lexeme: ";
    label += op.lexeme;
    std::string id = "assignment";
    id += std::to_string(node_counter++);
    std::vector<std::pair<std::string,std::string>> children_strs;
    for(Expr* arg : args) {
        children_strs.push_back(arg->ToString());
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

Unary::Unary(Token op, Expr* right): op(op), right(right) {}

std::pair<std::string, std::string> Unary::ToString() {
    std::string label = "Unary operator, lexeme: ";
    label += op.lexeme;
    std::string id = "assignment";
    id += std::to_string(node_counter++);
    std::pair<std::string,std::string> children = right->ToString();
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

Var::Var(Token name): name(name) {}

std::pair<std::string, std::string> Var::ToString() {
    std::string label = "VAR ";
    label += name.lexeme;
    std::string id = "assignment";
    id += std::to_string(node_counter++);
    std::string g = "";
    g += id;
    g += "[";
    g += "label=\"";
    g += label;
    g += "\", shape=\"square\"]\n";
    return std::make_pair(id, g);
}


Assign::~Assign() {
    delete value;
}

Binary::~Binary() {
    delete left;
    delete right;
}

Func::~Func() {
    for (auto ptr : args) {
        delete ptr;
    }
}

Group::~Group() {
    delete expr;
}

Literal::~Literal() {}

Logical::~Logical() {
    delete left;
    delete right;
}

Nil::~Nil() {}

Op::~Op() {
    for (auto ptr : args) {
        delete ptr;
    }
}

Unary::~Unary() {
    delete right;
}

Var::~Var() {}
