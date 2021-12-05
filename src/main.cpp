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

#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>

#include "lexer.hpp"
#include "parser.hpp"
#include "environment.hpp"

// We wrap this in an "extern" so that we can access it from
// JavaScript

extern "C" {
  inline char* as_c_string(const std::string& str) {
    char* buf = new char[str.length() + 1];
    buf[str.length()] = '\0';
    memcpy(buf, str.data(), str.length());
    return buf;
  }

  char* execute_program(char* input) {
    std::string read (input);
    Lexer lexer;
    std::vector<Token> tokens;
    try {
      tokens = lexer.lex(read);
    } catch(const std::exception& e) {
      return as_c_string(e.what());
    }
    if(lexer.has_had_error()) { 
      return as_c_string(lexer.print_errors());  
    }
    Parser p(tokens);
    std::vector<Stmt*> program;
    try {
      program = p.parse();
    } catch(const std::exception& e) {
      return as_c_string(e.what());
    }
    std::stringstream out;
    Environment env {out};
    try {
      for (Stmt* stmt : program) env.execute_stmt(stmt);
      for (auto stmt : program) delete stmt;
      char* ptr = as_c_string(out.str());
      return ptr;
    } catch(const std::exception& e) {
      return as_c_string(e.what());
    }
  }
}

int main(int argc, char* argv[]) {
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
