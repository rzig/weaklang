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

#ifndef EXPR_H_
#define EXPR_H_

#include <vector>

#include "token.hpp"

class Expr {
public:
    virtual ~Expr();
    virtual std::pair<std::string, std::string> to_string() = 0; 
    static size_t node_counter;
    static std::pair<std::string, std::string> make_string(std::string label, Expr* child);
    static std::pair<std::string, std::string> make_string(std::string label, std::initializer_list<Expr*> children);
    static std::pair<std::string, std::string> make_string(std::string label, std::vector<Expr*> exprs);
};

class ArrAccess : public Expr {
public:
    ArrAccess(Expr* id, Token brack, std::vector<Expr*> idx);
    std::pair<std::string, std::string> to_string();
    ~ArrAccess();
    Expr* id;
    Token brack;
    std::vector<Expr*> idx;
};

class Assign : public Expr {
public:
    Assign(Token name, Expr* value);
    Assign(Token name, std::vector<Expr*> idx, Expr* value);
    std::pair<std::string, std::string> to_string();
    ~Assign();
    Token name;
    std::vector<Expr*> idx;
    Expr* value;
};

class Binary : public Expr {
public:
    Binary(Expr* left, Token op, Expr* right);
    std::pair<std::string, std::string> to_string();
    ~Binary();
    Expr* left;
    Token op; 
    Expr* right;
};

class Func : public Expr {
public:
    Func(Token func, Token paren, std::vector<Expr*> args);
    std::pair<std::string, std::string> to_string();
    ~Func();
    Token func;
    Token paren;
    std::vector<Expr*> args;
};

enum LiteralType {
    LITERAL_STRING,
    LITERAL_DOUBLE,
    LITERAL_BOOL,
    LITERAL_ARRAY
};

class Literal : public Expr {
public: 
    Literal(Token token, std::string val);
    Literal(Token token, double val);
    Literal(Token token, bool val);
    Literal(Token token, std::vector<Expr*> vals);
    std::pair<std::string, std::string> to_string();
    ~Literal();
    Token token;
    LiteralType literal_type;
    std::string string_val;
    double double_val;
    bool bool_val;
    std::vector<Expr*> array_vals;
    
};

class Unary : public Expr {
public:
    Unary(Token op, Expr* right);
    std::pair<std::string, std::string> to_string();
    ~Unary();
    Token op;
    Expr* right;
};

class Var : public Expr {
public:
    Var(Token name);
    std::pair<std::string, std::string> to_string();
    ~Var();
    Token name;
};

class Nil : public Expr {
public:
    Nil();
    std::pair<std::string, std::string> to_string();
    ~Nil();
};

#endif // EXPR_H_
