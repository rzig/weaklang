#ifndef PARSER_H_
#define PARSER_H_

#include "token.hpp"
#include "stmt.hpp"
#include "expr.hpp"

#include <stdexcept>
#include <vector>

#define MAX_ARGS 64

class Parser {
public:
    Parser(std::vector<Token> input);
    std::vector<Stmt*> parse();
private:
    std::vector<Token> tokens;
    size_t cur_index = 0;

    bool match(TokenType type);
    bool match(TokenType type1, TokenType type2);
    bool match(std::initializer_list<TokenType> types);
    Token consume(TokenType type, std::string message);
    std::string create_error(Token token, std::string message);

    Stmt* declaration();
    Stmt* funDeclaration();
    Stmt* opDeclaration();
    Stmt* varDeclaration();
    Stmt* statement();
    std::vector<Stmt*> block();

    Stmt* exprStatement();
    Stmt* ifStatement();
    Stmt* printStatement();
    Stmt* whileStatement();
    Stmt* returnStatement();

    Expr* expression();
    Expr* function();
    Expr* operation();
    Expr* assignment();
    Expr* logicOr();
    Expr* logicAnd();
    Expr* equality();
    Expr* comparison();
    Expr* term();
    Expr* factor();
    Expr* unary();
    Expr* primary();
};

#endif // PARSER_H_
