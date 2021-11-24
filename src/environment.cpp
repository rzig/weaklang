#include "environment.hpp"

Environment::Environment(): return_val(), hit_return(false) {}

void Environment::add_func(std::string name, FuncDecl* func) {
    func_symbol_table.insert(std::pair<std::string, FuncDecl*>(name, func));
}

void Environment::add_op(std::string name, OpDecl* op) {
    op_symbol_table.insert(std::pair<std::string, OpDecl*>(name, op));
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
    if (CAN_MAKE(Assign*, assign)_FROM(expr)) {
	runtime_assert(VAR_EXISTS(assign->name.lexeme), assign->name, "Identifier doesn't correspond to a declared variable name");
	Variable var = evaluate_expr(assign->value);
	var_symbol_table.at(assign->name.lexeme) = var;
	return var;
    }
    if (CAN_MAKE(Binary*, binary)_FROM(expr)) {
	Variable left_var = evaluate_expr(binary->left);
	Variable right_var = evaluate_expr(binary->right);
	switch (binary->op.type) {
	case IDENTIFIER: {
	    runtime_assert(OP_EXISTS(binary->op.lexeme), binary->op, "Identifier doesn't correspond to a defined operator name");
	    OpDecl* opDecl = op_symbol_table.at(binary->op.lexeme);
	    Environment env;
	    env.func_symbol_table = func_symbol_table;
	    env.op_symbol_table = op_symbol_table;
	    env.add_var(opDecl->left.lexeme, left_var);
	    env.add_var(opDecl->right.lexeme, right_var);
	    for (Stmt* stmt : opDecl->stmts) {
		env.execute_stmt(stmt);
	    }
	    return env.get_return_val();
	}
	case OR: {
	    runtime_assert(left_var.is_bool(), binary->op, "Left expression evaluates to non-boolean value");
	    runtime_assert(right_var.is_bool(), binary->op, "Right expression evaluates to non-boolean value");
	    return Variable(std::get<bool>(left_var.value) || std::get<bool>(right_var.value));
	}
	case AND: {
	    runtime_assert(left_var.is_bool(), binary->op, "Left expression evaluates to non-boolean value");
	    runtime_assert(right_var.is_bool(), binary->op, "Right expression evaluates to non-boolean value");
	    return Variable(std::get<bool>(left_var.value) && std::get<bool>(right_var.value));
	}
	case EQUALS_EQUALS: {
	    runtime_assert(left_var.value.index() == right_var.value.index(), binary->op, "Left and right expressions differ in type");
	    return left_var.value == right_var.value;
	}
	case EXCLA_EQUALS: {
	    runtime_assert(left_var.value.index() == right_var.value.index(), binary->op, "Left and right expressions differ in type");
	    return left_var.value != right_var.value;
	}
	case GREATER_EQUALS: {
	    runtime_assert(left_var.value.index() == right_var.value.index(), binary->op, "Left and right expressions differ in type");
	    return left_var.value >= right_var.value;
	}
	case GREATER: {
	    runtime_assert(left_var.value.index() == right_var.value.index(), binary->op, "Left and right expressions differ in type");
	    return left_var.value > right_var.value;
	}
	case LESSER_EQUALS: {
	    runtime_assert(left_var.value.index() == right_var.value.index(), binary->op, "Left and right expressions differ in type");
	    return left_var.value <= right_var.value;
	}
	case LESSER: {
	    runtime_assert(left_var.value.index() == right_var.value.index(), binary->op, "Left and right expressions differ in type");
	    return left_var.value < right_var.value;
	}
	case MINUS: ELEMENTWISE_OP(-)
	case PLUS: ELEMENTWISE_OP(+)
	case SLASH: ELEMENTWISE_OP(/)
	case STAR: ELEMENTWISE_OP(*)
	case AS_SHAPE: {
	    runtime_assert(left_var.is_ndarray(), binary->op, "Left expression isn't an ndarray");
	    runtime_assert(right_var.is_ndarray(), binary->op, "Right expression isn't an ndarray");
	    auto new_size_double = std::get<std::pair<std::vector<double>, std::vector<size_t>>>(right_var.value).first; 
	    std::vector<size_t> new_size;
	    for (size_t i = 0; i < new_size_double.size(); i++) {
		size_t casted = (size_t) new_size_double.at(i);
		runtime_assert((double) casted == new_size_double.at(i), binary->op, "An expression used in array size is not close to an integer");
		new_size.push_back(casted);
	    }
	    return Variable(std::pair<std::vector<double>, std::vector<size_t>>(std::get<std::pair<std::vector<double>, std::vector<size_t>>>(left_var.value).first, new_size));
	}
	case EXP: {
	    if (left_var.is_double() && right_var.is_double()) { 
		return Variable(pow(std::get<double>(left_var.value), std::get<double>(right_var.value))); 
	    } 
	    if (left_var.is_double() && right_var.is_ndarray()) { 
		auto right_arr = std::get<std::pair<std::vector<double>, std::vector<size_t>>>(right_var.value).first; 
		for (size_t i = 0; i < right_arr.size(); i++) { 
		    right_arr.at(i) = pow(std::get<double>(left_var.value), right_arr.at(i)); 
		} 
		return Variable(std::pair<std::vector<double>, std::vector<size_t>>(right_arr, std::get<std::pair<std::vector<double>, std::vector<size_t>>>(right_var.value).second)); 
	    } 
	    if (left_var.is_ndarray() && right_var.is_double()) { 
		auto left_arr = std::get<std::pair<std::vector<double>, std::vector<size_t>>>(left_var.value).first; 
		for (size_t i = 0; i < left_arr.size(); i++) { 
		    left_arr.at(i) = pow(left_arr.at(i), std::get<double>(right_var.value)); 
		} 
		return Variable(std::pair<std::vector<double>, std::vector<size_t>>(left_arr, std::get<std::pair<std::vector<double>, std::vector<size_t>>>(left_var.value).second)); 
	    } 
	    if (left_var.is_ndarray() && right_var.is_ndarray()) { 
		auto left_var_pair = std::get<std::pair<std::vector<double>, std::vector<size_t>>>(left_var.value); 
		auto right_var_pair = std::get<std::pair<std::vector<double>, std::vector<size_t>>>(right_var.value); 
		runtime_assert(left_var_pair.second == right_var_pair.second, binary->op, "Expressions evaluate to arrays of differing sizes"); 
		std::vector<double> zipped; 
		for (size_t i = 0; i < left_var_pair.first.size(); i++) { 
		    zipped.push_back(pow(left_var_pair.first.at(i), right_var_pair.first.at(i))); 
		} 
		return Variable(std::pair<std::vector<double>, std::vector<size_t>>(zipped, left_var_pair.second)); 
	    } 
	    runtime_assert(false, binary->op, "At least one of left and right expressions are neither numbers nor ndarrays");
	}
	default: runtime_assert(false, binary->op, "Invalid binary operator");
	}
    }
    if (CAN_MAKE(Func*, func)_FROM(expr)) {
	
    }
}

void Environment::runtime_assert(bool cond, Token loc, std::string error_msg) {
    if (!cond) throw std::runtime_error(create_error(error_msg, loc));
}

std::string Environment::create_error(std::string error_msg, Token loc) {
    return "Runtime error: " + error_msg + ", occurred at line " + std::to_string(loc.line) + " at column " + std::to_string(loc.col);
}
