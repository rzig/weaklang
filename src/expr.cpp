// This file is part of weak-lang.
// weak-lang is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
// weak-lang is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
// You should have received a copy of the GNU Affero General Public License
// along with weak-lang. If not, see <https://www.gnu.org/licenses/>.

#include "expr.hpp"
#include<string>

Expr::~Expr() {}

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

ArrAccess::ArrAccess(Expr* id, Token brack, std::vector<Expr*> idx): id(id), brack(brack), idx(idx) {}

std::pair<std::string, std::string> ArrAccess::to_string() {
    return make_string("Array access", idx);
}

Assign::Assign(Token name, Expr* value): name(name), value(value) {}

Assign::Assign(Token name, std::vector<Expr*> idx, Expr* value): name(name), idx(idx), value(value) {}

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

Literal::Literal(Token token, std::string val): token(token), string_val(val), literal_type(LITERAL_STRING) {}

Literal::Literal(Token token, double val): token(token), double_val(val), literal_type(LITERAL_DOUBLE) {}

Literal::Literal(Token token, bool val): token(token), bool_val(val), literal_type(LITERAL_BOOL) {}

Literal::Literal(Token token, std::vector<Expr*> vals): token(token), array_vals(vals), literal_type(LITERAL_ARRAY) {}

std::pair<std::string, std::string> Literal::to_string() {
    std::string label = "Literal ";
    if(literal_type == LITERAL_ARRAY) {
	return make_string("Literal Array", array_vals);
    } else if(literal_type == LITERAL_STRING) {
        label += string_val;
    } else if (literal_type == LITERAL_DOUBLE) {
        label += std::to_string(double_val);
    } else {
        label += std::to_string(bool_val);
    }
    return make_string(label, {});
}

Nil::Nil() {}

std::pair<std::string, std::string> Nil::to_string() {
    return make_string("NIL", {});
}

Unary::Unary(Token op, Expr* right): op(op), right(right) {}

std::pair<std::string, std::string> Unary::to_string() {
    return make_string("Unary " + op.lexeme, right);
}

Var::Var(Token name): name(name) {}

std::pair<std::string, std::string> Var::to_string() {
    return make_string("Variable " + name.lexeme, {});
}

ArrAccess::~ArrAccess() {
    delete id;
    for (auto ptr : idx) {
        delete ptr;
    }
}

Assign::~Assign() {
    delete value;
    for (auto id : idx) {
        delete id;
    }
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

Literal::~Literal() {
    for (auto expr : array_vals) delete expr;
}

Nil::~Nil() {}

Unary::~Unary() {
    delete right;
}

Var::~Var() {}
