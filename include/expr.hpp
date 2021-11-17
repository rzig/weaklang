#ifndef EXPR_H_
#define EXPR_H_

#include <vector>

#include "token.hpp"

class Expr {
public:
    virtual ~Expr();
    virtual std::pair<std::string, std::string> ToString();
    static size_t node_counter;
};

class Assign : public Expr {
public:
    Assign(Token name, Expr* value);
    std::pair<std::string, std::string> ToString();
    ~Assign();
private:
    Token name; 
    Expr* value;
};

class Binary : public Expr {
public:
    Binary(Expr* left, Token op, Expr* right);
    std::pair<std::string, std::string> ToString();
    ~Binary();
private:
    Expr* left;
    Token op; 
    Expr* right;
};

class Func : public Expr {
public:
    Func(Token func, Token paren, std::vector<Expr*> args);
    std::pair<std::string, std::string> ToString();
    ~Func();
private:
    Token func;
    Token paren;
    std::vector<Expr*> args;
};

class Group : public Expr {
public:
    Group(Expr* expr);
    std::pair<std::string, std::string> ToString();
    ~Group();
private:
    Expr* expr;
};

class Literal : public Expr {
public: 
    Literal(std::string val);
    Literal(double val);
    Literal(bool val);
    Literal(std::vector<double> vals);
    std::pair<std::string, std::string> ToString();
    ~Literal();
private:
    std::string string_val;
    double double_val;
    bool bool_val;
    std::vector<double> array_vals;
};

class Logical : public Expr {
public:
    Logical(Expr* left, Token op, Expr* right);
    std::pair<std::string, std::string> ToString();
    ~Logical();
private:
    Expr* left;
    Token op;
    Expr* right;
};

class Op : public Expr {
public:
    Op(Token op, Token paren, std::vector<Expr*> args);
    std::pair<std::string, std::string> ToString();
    ~Op();
private:
    Token op;
    Token paren;
    std::vector<Expr*> args;
};

class Unary : public Expr {
public:
    Unary(Token op, Expr* right);
    std::pair<std::string, std::string> ToString();
    ~Unary();
private:
    Token op;
    Expr* right;
};

class Var : public Expr {
public:
    Var(Token name);
    std::pair<std::string, std::string> ToString();
    ~Var();
private:
    Token name;
};

class Nil : public Expr {
public:
    Nil();
    std::pair<std::string, std::string> ToString();
    ~Nil();
};

#endif // EXPR_H_
