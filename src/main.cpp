#include <stdio.h>

#include <lexer.hpp>

int main(/*int argc, char *argv[]*/) {
    Lexer lexer;
    lexer.lex("1.44 + (abc +3 / (3711.01 ^1)); (5 + 1) == (3 - 2) >= 7 != cba #Comment + - * /\n(5 + 3)\n\"Hello world\"");

    return 0;
}
