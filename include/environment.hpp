#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <unordered_map>
#include <stdexcept>
#include <string>

#include "variable.hpp"
#include "parser.hpp"
#include "error.hpp"
#include "util.hpp"

#define FUNC_EXISTS(func) (func_symbol_table.find(func) != func_symbol_table.end())
#define OP_EXISTS(op) (op_symbol_table.find(op) != op_symbol_table.end())
#define VAR_EXISTS(var) (var_symbol_table.find(var) != var_symbol_table.end())

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
    void runtime_assert(bool cond, Token loc, std::string error_msg);
    std::string create_error(std::string error_msg, Token loc);
};

#endif // ENVIRONMENT_H_
