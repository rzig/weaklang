#include "parser.hpp"

Parser::Parser(std::vector<Token> input): tokens(input) {}

std::vector<Stmt*> Parser::parse() {
    std::vector<Stmt*> decls;
    while (cur_index < tokens.size()) {
        decls.push_back(declaration());
    }
    return decls;
}

bool Parser::match(TokenType type) {
    if (tokens.at(cur_index).type == type) {
        cur_index++;
        return true;
    }
    return false;
}

Stmt* Parser::declaration() {
    if (match(FUNCTION)) return varDeclaration();
    if (match(OPERATOR)) return varDeclaration();
    if (match(LET)) return varDeclaration();
    return statement();
}

Stmt* Parser::funDeclaration() {

}

Stmt* Parser::opDeclaration() {

}

Stmt* Parser::varDeclaration() {

}

Stmt* Parser::statement() {

}

Stmt* Parser::block() {

}

Stmt* Parser::exprStatement() {

}

Stmt* Parser::ifStatement() {

}

Stmt* Parser::printStatement() {

}

Stmt* Parser::whileStatement() {

}

Stmt* Parser::returnStatement() {

}

Expr* Parser::expression() {

}

Expr* Parser::assignment() {

}

Expr* Parser::logicOr() {

}

Expr* Parser::logicAnd() {

}

Expr* Parser::equality() {

}

Expr* Parser::comparison() {

}

Expr* Parser::term() {

}

Expr* Parser::factor() {

}

Expr* Parser::unary() {

}

Expr* Parser::primary() {

}
