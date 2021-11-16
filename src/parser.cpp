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

    consume(LEFT_BRACE, "Expected '{' before function body");
    std::vector<Stmt*> body = block();
    return new FuncDecl(name, params, body);
}

Stmt* Parser::opDeclaration() {
    Token name = consume(IDENTIFIER, "Expected operator name");

    consume(LEFT_PAREN, "Expected '(' after operator name");
    Token left = consume(IDENTIFIER, "Exepcted left parameter name in operator declaration");
    consume(COMMA, "Exepcted ',' after left operator parameter");
    Token right = consume(IDENTIFIER, "Expected right parameter name in operator declaration");
    consume(RIGHT_PAREN, "Expected ')' after operator parameters list");

    consume(LEFT_BRACE, "Expected '{' before operator body");
    std::vector<Stmt*> body = block();
    return new OpDecl(name, left, right, body);
}

Stmt* Parser::varDeclaration() {
    Token name = consume(IDENTIFIER, "Expected variable name");

    Expr* initialize = nullptr;
    if (match(EQUALS)) initialize = expression();

    consume(SEMI, "Expect semi-colon after variable declaration");
    return new VarDecl(name, initialize);
}

Stmt* Parser::statement() {
    if (match(IF)) return ifStatement();
    if (match(PRINT)) return printStatement();
    if (match(RETURN)) return returnStatement();
    if (match(WHILE)) return whileStatement();
    // if (match(LEFT_BRACE)) return new std::vector<Stmt*> stmt.block();
    return exprStatement();
}

std::vector<Stmt*> Parser::block() {
    std::vector<Stmt*> contents;
    while(tokens.at(cur_index).type != END && tokens.at(cur_index).type != RIGHT_BRACE) {
        contents.push_back(declaration());
    }

    if(tokens.at(cur_index).type == END) {
        create_error(tokens.at(cur_index), "Expected '}' after block");
    }

    consume(RIGHT_BRACE, "Expected '}' after block");
    return contents;
}

Stmt* Parser::exprStatement() {
    Expr* exp = expression();
    consume(SEMI, "Expected ';' after expression");
    Stmt* stmt = new ExprStmt(exp);
    return stmt;
}

Stmt* Parser::ifStatement() {
    consume(LEFT_PAREN, "Expected '(' before condition");
    Expr* condition = expression();
    consume(RIGHT_PAREN, "Expected ')' after condition");

    consume(LEFT_BRACE, "Expected '{' before contents of if statement");
    std::vector<Stmt*> body = block();
    return new If(condition, body);
}

Stmt* Parser::printStatement() {
    Stmt* stmt = new Print(tokens.at(cur_index - 1), expression());
    consume(SEMI, "Expected ';' after print statement");
    return stmt;
}

Stmt* Parser::whileStatement() {
    consume(LEFT_PAREN, "Expected '(' before condition");
    Expr* condition = expression();
    consume(RIGHT_PAREN, "Expected ')' after condition");

    consume(LEFT_BRACE, "Expected '{' before contents of while loop");
    std::vector<Stmt*> body = block();
    return new While(condition, body);
}

Stmt* Parser::returnStatement() {
    Stmt* stmt = new Return(tokens.at(cur_index - 1), expression());
    consume(SEMI, "Expected ';' after return statement");
    return stmt;
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
    // Not sure what this should return
}

Expr* Parser::unary() {
    switch(tokens.at(cur_index).type) {
        case EXCLA:
        case MINUS:
        case SHAPE: {
            Token t = tokens.at(cur_index);
            cur_index += 1;
            Expr* next = unary();
            return new Unary(t, next);
        }
        default:
            return primary();
    }
}

Expr* Parser::primary() {
    if(match(TRUE)) return new Literal(true);
    if(match(FALSE)) return new Literal(false);
    if(match(NIL)) return new Nil(tokens.at(cur_index-1));
    if(match(NUMBER)) return new Literal(tokens.at(cur_index-1).literal_double);
    if(match(STRING)) return new Literal(tokens.at(cur_index-1).literal_string);
    if(match(IDENTIFIER)) return new Var(tokens.at(cur_index-1));
    if(match(LEFT_PAREN)) {
        Expr* exp = expression();
        consume(RIGHT_PAREN, "Expected ')' after expression");
        return exp;
    }
    if(match(LEFT_BRACK)) {
        std::vector<double> array_vals;
        while(tokens.at(cur_index).type != END && tokens.at(cur_index).type != RIGHT_BRACK) {
            if(array_vals.size() > 0) {
                consume(COMMA, "Expected ',' between values in array");
            }
            Token number_token = consume(NUMBER, "Expected numeric value in array");
            array_vals.push_back(number_token.literal_double);
        }
	return new Literal(array_vals);
    }
    throw std::runtime_error(create_error(tokens.at(cur_index), "Expected primary"));
}
