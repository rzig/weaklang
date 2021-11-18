#include <fstream>
#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"

// testing the simplest possible assignment
int test_assignment() {
  std::string program = "a x = 5;";
  Lexer lex; 
  std::vector<Token> tokens = lex.lex(program); 
  /* for(auto& token : tokens){
    std::cout << print_token_type(token.type) << ": " << token.lexeme << std::endl; 
  } */
  Parser p{tokens}; 
  std::vector<Stmt*> statements = p.parse(); 
  // std::cout << p.as_dot() << std::endl; 

  for(auto* s : statements)
    delete s;
  return 0; 
}

int main(int argc, char* argv[]) {
  return test_assignment();
  
  if (argc == 1) {
    std::cout << "Usage: " << argv[0] << " INPUT_FILE" << std::endl;
    return 1;
  }
  for (size_t i = 1; i < (size_t)argc; i++) {
    std::ifstream input_file(argv[i]);
    if (input_file.is_open()) {
      std::string read((std::istreambuf_iterator<char>(input_file)),
                       (std::istreambuf_iterator<char>()));
      Lexer lexer;
      std::vector<Token> tokens = lexer.lex(read);
      Parser p {tokens};
      p.parse();
      std::cout << p.as_dot() << std::endl;
    } else {
      std::cout << "Couldn't open file " << argv[i] << ". Quitting."
                << std::endl;
      return 1;
    }
  }
  return 0;
}
