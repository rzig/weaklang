#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <unordered_map>
#include <string>

#include "variable.hpp"
#include "parser.hpp"

class Environment {
public:
    Environment();
    ~Environment() = default;
    void add_func(std::string name, Stmt* func);
    void add_op(std::string name, Stmt* op);
    void add_var(std::string name, Variable var);
    bool has_hit_return();
    Variable get_return_val();
    void execute_stmt(Stmt* stmt);
private:
    std::unordered_map<std::string, Stmt*> func_symbol_table; 
    std::unordered_map<std::string, Stmt*> op_symbol_table; 
    std::unordered_map<std::string, Variable> var_symbol_table;
    bool hit_return;
    Variable return_val;
    Variable evaluate_expr(Expr* expr);
};

#endif // ENVIRONMENT_H_
