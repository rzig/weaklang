#ifndef LEXER_H_
#define LEXER_H_

#include <iostream>
#include <vector>

#include "token.hpp"

class Lexer {

public:
    void lex(const std::string &to_lex);

};

#endif // LEXER_H_
