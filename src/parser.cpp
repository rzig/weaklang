#include "parser.hpp"

Parser::Parser(std::vector<Token> input): tokens(input) {}

Expr Parser::expression() {
    // TODO why is this in book
    // but commented out in finalized repo
    return equality(); 
}

Expr Parser::equality() {
    // TODO figure out this
}