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
    if (CAN_MAKE(ArrAccess*, arrAccess)_FROM(expr)) {
	runtime_assert(VAR_EXISTS(arrAccess->id.lexeme), arrAccess->id, "Identifier doesn't correspond to a declared variable name");
	Variable var = var_symbol_table.at(arrAccess->id.lexeme);
	runtime_assert(var.is_ndarray(), arrAccess->id, "Identifier in array access isn't an ndarray");
	auto arr = std::get<std::pair<std::vector<double>, std::vector<size_t>>>(var.value);
	runtime_assert(arr.second.size() == arrAccess->idx.size(), arrAccess->brack, "Number of dimensions in array element access differs from number of dimensions in array");
	std::vector<size_t> indices;
	for (size_t i = 0; i < arrAccess->idx.size(); i++) {
	    Expr* index = arrAccess->idx.at(i);
	    Variable index_val = evaluate_expr(index);
	    runtime_assert(index_val.is_double(), arrAccess->brack, "An expression used in array indexing is not a number");
	    size_t casted = (size_t) std::get<double>(index_val.value);
	    runtime_assert((double) casted == std::get<double>(index_val.value), arrAccess->brack, "An expression used in array indexing is not close to an integer");
	    runtime_assert(casted < arr.second.at(i), arrAccess->brack, "An expression used in array indexing is larger than a dimension of the ndarray");
	    indices.push_back(casted);
	}
	size_t flat_index = indices[0];
	for (size_t i = 1; i < indices.size(); i++) {
	    flat_index = indices[i] + flat_index * arr.second[i - 1];
	}
	return Variable(arr.first.at(flat_index));
    }
}

void Environment::runtime_assert(bool cond, Token loc, std::string error_msg) {
    if (!cond) throw std::runtime_error(create_error(error_msg, loc));
}

std::string Environment::create_error(std::string error_msg, Token loc) {
    return "Runtime error: " + error_msg + ", occurred at line " + std::to_string(loc.line) + " at column " + std::to_string(loc.col);
}
