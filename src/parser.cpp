#include "parser.hpp"
#include <iostream>

Parser::Parser(std::vector<Token> input): tokens(input) {}

std::vector<Stmt*> Parser::parse() {
    std::vector<Stmt*> decls;
    while (cur_index < tokens.size() && tokens.at(cur_index).type != END) {
        for (auto t : tokens) {
            std::cout << print_token_type(t.type) << " ";
        }
        std::cout << std::endl;
        std::cout << "curr: " << cur_index << std::endl;
        std::cout << "size: " << tokens.size() << std::endl;
        std::cout << print_token_type(tokens.at(cur_index).type) << std::endl;
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

bool Parser::match(TokenType type1, TokenType type2) {
    return match(type1) || match(type2);
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for(auto t : types) {
        if(match(t)) {
            return true;
        }
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
            std::cout << "broken is " << print_token_type(tokens.at(cur_index).type) << std::endl;
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
    Expr* expr;
    if (tokens.at(cur_index - 1).type != SEMI) expr = expression();
    else expr = new Nil();
    Stmt* stmt = new Return(tokens.at(cur_index - 1), expr);
    consume(SEMI, "Expected ';' after return statement");
    return stmt;
}

Expr* Parser::expression() {
    return function();
}

Expr* Parser::function() {
    if(tokens.at(cur_index).type == IDENTIFIER && cur_index < tokens.size() - 1 && tokens.at(cur_index+1).type == LEFT_PAREN) {
        Token name = consume(IDENTIFIER, "");
        Token left_p = consume(LEFT_PAREN, "");
        std::vector<Expr*> args;
        while(cur_index < tokens.size() && tokens.at(cur_index).type == IDENTIFIER) {
            if (args.size() == 0) {
                Expr* arg = expression();
                args.push_back(arg);
            }
            else if (args.size() < MAX_ARGS) {
                consume(COMMA, "Expected comma in function call");
                Expr* arg = expression();
                args.push_back(arg);
            }
        }
        return new Func(name, left_p, args);
    } else {
        return operation();
    }
}

Expr* Parser::operation() {
    Expr* exp = assignment();
    while(match(IDENTIFIER)) {
        Token id = tokens.at(cur_index-1);
        Expr* right = assignment();
        exp = new Binary(exp, id, right);
    }
    return exp;
}

Expr* Parser::assignment() {
    if(tokens.at(cur_index).type == IDENTIFIER) {
        Token id = consume(IDENTIFIER, "Expected identifier");
        consume(EQUALS, "Expected '=' after identifier");
        Expr* right = assignment();
        return new Assign(id, right);
    } else {
        return logicOr();
    }
}

Expr* Parser::logicOr() {
  Expr* exp = logicAnd();
  while (match(OR)) {
    Token t = tokens.at(cur_index - 1);
    Expr* next = logicAnd();
    exp = new Binary(exp, t, next);
  }
  return exp;
}

Expr* Parser::logicAnd() {
  Expr* exp = equality();
  while (match(AND)) {
    Token a = tokens.at(cur_index - 1);
    Expr* next = equality();
    exp = new Binary(exp, a, next);
  }
  return exp;
}

Expr* Parser::equality() {
  Expr* exp = comparison();
  while (match({EQUALS_EQUALS, EQUALS})) {
    Token t = tokens.at(cur_index - 1);
    Expr* next = comparison();
    exp = new Binary(exp, t, next);
  }
  return exp; 
}

Expr* Parser::comparison() {
  Expr* exp = term();
  while(match({GREATER_EQUALS, GREATER, LESSER_EQUALS, LESSER})) {
      Token comp = tokens.at(cur_index - 1);
      Expr* next = term();
      exp = new Binary(exp, comp, next);
  }
  return exp;
}

Expr* Parser::term() {
    Expr* left = factor();
    while(match({MINUS, PLUS})) {
        Token op = tokens.at(cur_index-1);
        Expr* fac = factor();
        left = new Binary(left, op, fac);
    }
    return left;
}

Expr* Parser::factor() {
    Expr* left = unary();
    while(match({SLASH, STAR, AT, AS_SHAPE})) {
        Token op = tokens.at(cur_index-1);
        Expr* un = unary();
        left = new Binary(left, op, un);
    }
    return left;
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
    if(match(NIL)) return new Nil();
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
