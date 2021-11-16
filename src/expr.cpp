#include "expr.hpp"

Expr::~Expr() {}

Assign::Assign(Token name, Expr* value): name(name), value(value) {}

Binary::Binary(Expr* left, Token op, Expr* right): left(left), op(op), right(right) {}

Func::Func(Token func, Token paren, std::vector<Expr*> args): func(func), paren(paren), args(args) {}

Group::Group(Expr* expr): expr(expr) {}

Literal::Literal(std::string val): string_val(val) {}

Literal::Literal(double val): double_val(val) {}

Literal::Literal(bool val): bool_val(val) {}

Literal::Literal(std::vector<double> vals): array_vals(vals) {}

Logical::Logical(Expr* left, Token op, Expr* right): left(left), op(op), right(right) {}

Nil::Nil(Token token): token(token) {}

Op::Op(Token op, Token paren, std::vector<Expr*> args): op(op), paren(paren), args(args) {}

Unary::Unary(Token op, Expr* right): op(op), right(right) {}

Var::Var(Token name): name(name) {}

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
