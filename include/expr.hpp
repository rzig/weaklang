#ifndef EXPR_H_
#define EXPR_H_

#include <vector>

#include "token.hpp"

class Expr {
public:
    virtual ~Expr();
    virtual std::pair<std::string, std::string> to_string();
    static size_t node_counter;
    static std::pair<std::string, std::string> make_string(std::string label, Expr* child);
    static std::pair<std::string, std::string> make_string(std::string label, std::initializer_list<Expr*> children);
    static std::pair<std::string, std::string> make_string(std::string label, std::vector<Expr*> exprs);
};

class Assign : public Expr {
public:
    Assign(Token name, Expr* value);
    std::pair<std::string, std::string> to_string();
    ~Assign();
private:
    Token name; 
    Expr* value;
};

class Binary : public Expr {
public:
    Binary(Expr* left, Token op, Expr* right);
    std::pair<std::string, std::string> to_string();
    ~Binary();
private:
    Expr* left;
    Token op; 
    Expr* right;
};

class Func : public Expr {
public:
    Func(Token func, Token paren, std::vector<Expr*> args);
    std::pair<std::string, std::string> to_string();
    ~Func();
private:
    Token func;
    Token paren;
    std::vector<Expr*> args;
};

class Group : public Expr {
public:
    Group(Expr* expr);
    std::pair<std::string, std::string> to_string();
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
    std::pair<std::string, std::string> to_string();
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
    std::pair<std::string, std::string> to_string();
    ~Logical();
private:
    Expr* left;
    Token op;
    Expr* right;
};

class Op : public Expr {
public:
    Op(Token op, Token paren, std::vector<Expr*> args);
    std::pair<std::string, std::string> to_string();
    ~Op();
private:
    Token op;
    Token paren;
    std::vector<Expr*> args;
};

class Unary : public Expr {
public:
    Unary(Token op, Expr* right);
    std::pair<std::string, std::string> to_string();
    ~Unary();
private:
    Token op;
    Expr* right;
};

class Var : public Expr {
public:
    Var(Token name);
    std::pair<std::string, std::string> to_string();
    ~Var();
private:
    Token name;
};

class Nil : public Expr {
public:
    Nil();
    std::pair<std::string, std::string> to_string();
    ~Nil();
};

#endif // EXPR_H_
