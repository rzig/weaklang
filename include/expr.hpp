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
    ArrAccess(Token id, Token brack, std::vector<Expr*> idx);
    std::pair<std::string, std::string> to_string();
    ~ArrAccess();
    Token id;
    Token brack;
    std::vector<Expr*> idx;
};

class Assign : public Expr {
public:
    Assign(Token name, Expr* value);
    std::pair<std::string, std::string> to_string();
    ~Assign();
    Token name; 
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

class Group : public Expr {
public:
    Group(Expr* expr);
    std::pair<std::string, std::string> to_string();
    ~Group();
    Expr* expr;
};

enum LiteralType {
    LITERAL_STRING,
    LITERAL_DOUBLE,
    LITERAL_BOOL,
    LITERAL_ARRAY
};

class Literal : public Expr {
public: 
    Literal(std::string val);
    Literal(double val);
    Literal(bool val);
    Literal(std::vector<Expr*> vals);
    std::pair<std::string, std::string> to_string();
    ~Literal();
    LiteralType literal_type;
    std::string string_val;
    double double_val;
    bool bool_val;
    std::vector<Expr*> array_vals;
    
};

class Logical : public Expr {
public:
    Logical(Expr* left, Token op, Expr* right);
    std::pair<std::string, std::string> to_string();
    ~Logical();
    Expr* left;
    Token op;
    Expr* right;
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
