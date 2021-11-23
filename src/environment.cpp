#include "environment.hpp"

Environment::Environment(): return_val(), hit_return(false) {}

void Environment::add_func(std::string name, Stmt* func) {
    func_symbol_table.insert(std::pair<std::string, Stmt*>(name, func));
}

void Environment::add_op(std::string name, Stmt* op) {
    op_symbol_table.insert(std::pair<std::string, Stmt*>(name, op));
}

void Environment::add_var(std::string name, Variable var) {
    var_symbol_table.insert(std::pair<std::string, Variable>(name, var));
}

bool Environment::has_hit_return() {
    return hit_return;
}

Variable Environment::get_return_val() {
    return return_val;
}

void Environment::execute_stmt(Stmt* stmt) {

}

Variable Environment::evaluate_expr(Expr* expr) {

}
