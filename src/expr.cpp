#include "expr.hpp"

Assign::Assign(Token name, Expr value): name(name), value(value) {}

Binary::Binary(Expr left, Token op, Expr right): left(left), op(op), right(right) {}

Literal::Literal(std::string val): val(val) {}

Logical::Logical(Expr left, Token op, Expr right): left(left), op(op), right(right) {} 