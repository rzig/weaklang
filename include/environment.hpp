#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <unordered_map>
#include <string>

#include "variable.hpp"
#include "parser.hpp"

class Environment {
public:
    Environment() = default;
    ~Environment() = default;
    void add_var(Variable var);
private:
    std::unordered_map<std::string, Stmt*> func_symbol_table; 
    std::unordered_map<std::string, Stmt*> op_symbol_table; 
    std::unordered_map<std::string, Variable> variables;
};

#endif // ENVIRONMENT_H_
