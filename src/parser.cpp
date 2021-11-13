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

Token Parser::consume(TokenType type, std::string message) {
    if (tokens.at(cur_index).type == type) return tokens.at(cur_index++);
    throw std::runtime_error(create_error(tokens.at(cur_index), message));
}

std::string Parser::create_error(Token token, std::string message) {
    return message + " : \"" + token.lexeme + "\", at line " + std::to_string(token.line) + " and column " + std::to_string(token.col);
}

Stmt* Parser::declaration() {
    if (match(FUNCTION)) return funDeclaration();
    if (match(OPERATOR)) return opDeclaration();
    if (match(LET)) return varDeclaration();
    return statement();
}

Stmt* Parser::funDeclaration() {
    Token name = consume(IDENTIFIER, "Expected function name");

    consume(LEFT_PAREN, "Expected '(' after function name");
    std::vector<Token> params;
    while (tokens.at(cur_index).type != END && tokens.at(cur_index).type != RIGHT_PAREN) {
        if (params.size() == 0) {
            params.push_back(consume(IDENTIFIER, "Expected parameter name in function declaration"));
        }
        else if (params.size() < MAX_ARGS) {
            consume(COMMA, "Expected comma in function declaration");
            params.push_back(consume(IDENTIFIER, "Expected parameter name in function declaration"));
        }
        else {
            throw std::runtime_error(create_error(name, "Too many arguments for function (max " + std::to_string(MAX_ARGS) + ")"));
        }
    }
    consume(RIGHT_PAREN, "Expected ')' after parameters of function");
}

Stmt* Parser::opDeclaration() {

}

Stmt* Parser::varDeclaration() {
    Token name = consume(IDENTIFIER, "Expected variable name");

    Expr* initialize = nullptr;
    if (match(EQUALS)) initialize = expression();

    consume(SEMI, "Expect semi-colon after variable declaration");
    return new VarDecl(name, initialize);
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
