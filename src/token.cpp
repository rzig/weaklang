// This file is part of weak-lang.
// weak-lang is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
// weak-lang is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
// You should have received a copy of the GNU Affero General Public License
// along with weak-lang. If not, see <https://www.gnu.org/licenses/>.

#include "token.hpp"

Token::Token(TokenType type_in, std::string lexeme_in, size_t line_in, size_t col_in) :
    type(type_in),
    lexeme(lexeme_in),
    line(line_in),
    col(col_in),
    literal_double(type_in == NUMBER ? std::stod(lexeme_in) : 0),
    literal_string(type_in == STRING ? lexeme_in : "") {}

std::string print_token_type(TokenType type) {
    switch (type) {
        case PLUS: return std::string("PLUS");
        case MINUS: return std::string("MINUS");
        case STAR: return std::string("STAR");
        case SLASH: return std::string("SLASH");
        case EXP: return std::string("EXP");
        case AT: return std::string("AT");
        case COMMA: return std::string("COMMA");
        case SEMI: return std::string("SEMI");
        case LEFT_PAREN: return std::string("LEFT_PAREN");
        case RIGHT_PAREN: return std::string("RIGHT_PAREN");
        case LEFT_BRACE: return std::string("LEFT_BRACE");
        case RIGHT_BRACE: return std::string("RIGHT_BRACE");
        case LEFT_BRACK: return std::string("LEFT_BRACK");
        case RIGHT_BRACK: return std::string("RIGHT_BRACK");
        case EXCLA: return std::string("EXCLA");
        case EQUALS: return std::string("EQUALS");
        case GREATER: return std::string("GREATER");
        case LESSER: return std::string("LESSER");
        case EXCLA_EQUALS: return std::string("EXCLA_EQUALS");
        case EQUALS_EQUALS: return std::string("EQUALS_EQUALS");
        case GREATER_EQUALS: return std::string("GREATER_EQUALS");
        case LESSER_EQUALS: return std::string("LESSER_EQUALS");
        case AND: return std::string("AND");
        case OR: return std::string("OR");
        case TRUE: return std::string("TRUE");
        case FALSE: return std::string("FALSE");
        case FUNCTION: return std::string("FUNCTION");
        case OPERATOR: return std::string("OPERATOR");
        case NIL: return std::string("NIL");
        case IF: return std::string("IF");
        case PRINT: return std::string("PRINT");
        case RETURN: return std::string("RETURN");
        case LET: return std::string("LET");
        case WHILE: return std::string("WHILE");
        case END: return std::string("END");
        case IDENTIFIER: return std::string("IDENTIFIER");
        case NUMBER: return std::string("NUMBER");
        case BOOLEAN: return std::string("BOOLEAN");
        case STRING: return std::string("STRING");
        case SHAPE: return std::string("SHAPE");
        case AS_SHAPE: return std::string("AS_SHAPE");
        case EMPTY: return std::string("EMPTY");
        case ASSERT: return std::string("ASSERT"); 
    }
    return "";
}
