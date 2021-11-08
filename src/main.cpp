#include <stdio.h>

#include <lexer.hpp>

int main(/*int argc, char *argv[]*/) {
    Lexer lexer;
    lexer.lex("1 + (5 +3 / (3 ^1));");

    return 0;
}
