#include <iostream>
#include <fstream>

#include "lexer.hpp"

int main(int argc, char *argv[]) {
    for (size_t i = 1; i < (size_t) argc; i++) {
        std::ifstream input_file(argv[i]);
        if (input_file.is_open()) {
            std::string read((std::istreambuf_iterator<char>(input_file)), (std::istreambuf_iterator<char>()));
            Lexer lexer;
            lexer.lex(read);
        }
        else {
            std::cout << "Couldn't open file " << argv[i] << ". Quitting." << std::endl;
            return 1;
        }
    }
    return 0;
}
