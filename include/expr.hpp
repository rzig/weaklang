#ifndef EXPR_H_
#define EXPR_H_

#include <vector>

#include "token.hpp"

class Expr {};

class Assign : public Expr {
public:
    Assign(Token name, Expr value); 
private:
    Token name; 
    Expr value; 
};

class Binary : public Expr {
public:
    Binary(Expr left, Token op, Expr right);
private:
    Expr left; 
    Token op; 
    Expr right; 
};

class Func : public Expr {
public:
    Func(Token func, Token paren, std::vector<Expr> args);
private:
    Token func;
    Token paren;
    std::vector<Expr> args;
};

class Group : public Expr {
public:
    Group(Expr expr);
private:
    Expr expr;
};

class Literal : public Expr {
public: 
    Literal(std::string val);
    Literal(double val);
    Literal(bool val);
private:
    std::string string_val;
    double double_val;
    bool bool_val;
};

class Logical : public Expr {
public:
    Logical(Expr left, Token op, Expr right); 
private:
    Expr left; 
    Token op;
    Expr right; 
};

class Op : public Expr {
public:
    Op(Token op, Token paren, std::vector<Expr> args);
private:
    Token op;
    Token paren;
    std::vector<Expr> args;
};

class Unary : public Expr {
public:
    Unary(Token op, Expr right);
private:
    Token op;
    Expr right;
};

class Var : public Expr {
public:
    Var(Token name);
private:
    Token name;
};

#endif // EXPR_H_
