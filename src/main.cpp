#include <fstream>
#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"
#include "environment.hpp"

int main(int argc, char* argv[]) {
  if (argc == 1) {
    std::cout << "Usage: " << argv[0] << " INPUT_FILE" << std::endl;
    return 1;
  }
  for (size_t i = 1; i < (size_t)argc; i++) {
    std::ifstream input_file(argv[i]);
    if (input_file.is_open()) {
      std::string read = "v T;"; /* read((std::istreambuf_iterator<char>(input_file)),
                       (std::istreambuf_iterator<char>())); */
      Lexer lexer;
      std::vector<Token> tokens = lexer.lex(read);
      if(lexer.has_had_error()) { 
        std::cout << lexer.print_errors();  
        return 1; // if errors in syntax, don't continue to parse
      }
      Parser p(tokens);
      std::vector<Stmt*> program = p.parse();
      //std::cout << p.as_dot() << std::endl;
      Environment env;
      for (Stmt* stmt : program) env.execute_stmt(stmt);
      for (auto stmt : program) delete stmt;
    } else {
      std::cout << "Couldn't open file " << argv[i] << ". Quitting."
                << std::endl;
      return 1;
    }
  }
  return 0;
}
