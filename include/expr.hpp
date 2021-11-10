#ifndef EXPR_H_
#define EXPR_H_

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

class Literal : public Expr {
public: 
    Literal(std::string val); 
private:
    std::string val; 
};

class Logical : public Expr {
public:
    Logical(Expr left, Token op, Expr right); 
private:
    Expr left; 
    Token op;
    Expr right; 
};

#endif // EXPR_H_