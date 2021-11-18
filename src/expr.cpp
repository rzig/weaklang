#include "expr.hpp"
#include<string>

Expr::~Expr() {}

/* std::pair<std::string, std::string> Expr::to_string() {
    std::string a = "";
    std::string b = "";

    return std::make_pair(a,b);
} */

std::pair<std::string, std::string> Expr::make_string(std::string label, std::vector<Expr*> children) {
    std::string id = "expression";
    id += std::to_string(node_counter++);
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

std::pair<std::string, std::string> Expr::make_string(std::string label, std::initializer_list<Expr*> children) {
    std::vector<Expr*> child_vec;
    for(auto* e : children) {
        child_vec.push_back(e);
    }
    return make_string(label, child_vec);
}

std::pair<std::string, std::string> Expr::make_string(std::string label, Expr* child) {
    std::vector<Expr*> c;
    c.push_back(child);
    return make_string(label, c);
}

size_t Expr::node_counter = 0;

Assign::Assign(Token name, Expr* value): name(name), value(value) {}

std::pair<std::string, std::string> Assign::to_string() {
    return make_string("Assignment of " + name.lexeme, value);
}

Binary::Binary(Expr* left, Token op, Expr* right): left(left), op(op), right(right) {}

std::pair<std::string, std::string> Binary::to_string() {
    return make_string("Binary operator " + op.lexeme, {left, right});
}

Func::Func(Token func, Token paren, std::vector<Expr*> args): func(func), paren(paren), args(args) {}

std::pair<std::string, std::string> Func::to_string() {
    return make_string("Function call to " + func.lexeme, args);
}

Group::Group(Expr* expr): expr(expr) {}

std::pair<std::string, std::string> Group::to_string() {
    return make_string("Group", expr);
}

Literal::Literal(std::string val): string_val(val) {}

Literal::Literal(double val): double_val(val) {}

Literal::Literal(bool val): bool_val(val) {}

Literal::Literal(std::vector<double> vals): array_vals(vals) {}

std::pair<std::string, std::string> Literal::to_string() {
    std::string label = "Literal ";
    if(not array_vals.empty()) {
        for(auto v : array_vals) {
            label += std::to_string(v);
            label += ", ";
        }
    } else if(not string_val.empty()) {
        label += string_val;
    } else if (not (double_val == 0)) {
        label += std::to_string(double_val);
    } else {
        label += std::to_string(bool_val);
    }
    return make_string(label, {});
}

Logical::Logical(Expr* left, Token op, Expr* right): left(left), op(op), right(right) {}

std::pair<std::string, std::string> Logical::to_string() {
    return make_string("Logical operator " + op.lexeme, {left, right});
}

Nil::Nil() {}

std::pair<std::string, std::string> Nil::to_string() {
    return make_string("NIL", {});
}

Op::Op(Token op, Token paren, std::vector<Expr*> args): op(op), paren(paren), args(args) {}

std::pair<std::string, std::string> Op::to_string() {
    return make_string("Operation " + op.lexeme, args);
}

Unary::Unary(Token op, Expr* right): op(op), right(right) {}

std::pair<std::string, std::string> Unary::to_string() {
    return make_string("Unary " + op.lexeme, right);
}

Var::Var(Token name): name(name) {}

std::pair<std::string, std::string> Var::to_string() {
    return make_string("Variable " + name.lexeme, {});
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
