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
    if (CAN_MAKE(ExprStmt*, exprStmt)_FROM(stmt)) {
	evaluate_expr(exprStmt->expr);
    }
    else if (CAN_MAKE(FuncDecl*, funcDecl)_FROM(stmt)) {
	add_func(funcDecl->name.lexeme, funcDecl);
    }
    else if (CAN_MAKE(If*, ifStmt)_FROM(stmt)) {
	Variable cond = evaluate_expr(ifStmt->cond);
	runtime_assert(cond.is_bool(), ifStmt->keyword, "If statement expected a boolean condition");
	if (std::get<bool>(cond.value)) {
	    for (Stmt* stmtInIf : ifStmt->stmts) {
		execute_stmt(stmtInIf);
	    }
	}
    }
    else if (CAN_MAKE(OpDecl*, opDecl)_FROM(stmt)) {
	add_op(opDecl->name.lexeme, opDecl);
    }
    else if (CAN_MAKE(Print*, print)_FROM(stmt)) {
	Variable to_print = evaluate_expr(print->expr);
	if (to_print.is_bool()) std::cout << (std::get<bool>(to_print.value) ? "True" : "False") << std::endl;
	else if (to_print.is_double()) std::cout << std::get<double>(to_print.value) << std::endl;
	else if (to_print.is_string()) std::cout << std::get<std::string>(to_print.value) << std::endl;
    }
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
    else if (CAN_MAKE(Assign*, assign)_FROM(expr)) {
	runtime_assert(VAR_EXISTS(assign->name.lexeme), assign->name, "Identifier doesn't correspond to a declared variable name");
	Variable var = evaluate_expr(assign->value);
	var_symbol_table.at(assign->name.lexeme) = var;
	return var;
    }
    else if (CAN_MAKE(Binary*, binary)_FROM(expr)) {
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
	case AT: {
	    runtime_assert(left_var.is_ndarray(), binary->op, "Left expression isn't an ndarray");
	    runtime_assert(right_var.is_ndarray(), binary->op, "Right expression isn't an ndarray");
	    auto extract_left = std::get<std::pair<std::vector<double>, std::vector<size_t>>>(left_var.value); 
	    auto extract_right = std::get<std::pair<std::vector<double>, std::vector<size_t>>>(right_var.value); 
	    runtime_assert(extract_left.second.size() == 2, binary->op, "Left expression isn't a 2d ndarray");
	    runtime_assert(extract_right.second.size() == 2, binary->op, "Left expression isn't a 2d ndarray");
	    runtime_assert(extract_left.second.at(1) == extract_right.second.at(0), binary->op, "Left array's num of cols differs from right array's num of rows");
	    size_t r = extract_left.second.at(0);
	    size_t m = extract_left.second.at(1);
	    size_t c = extract_right.second.at(1);
	    std::vector<double> result;
	    result.reserve(r * c);
	    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, r, c, m, 1., extract_left.first.data(), m, extract_right.first.data(), c, 0., result.data(), c);
	    return Variable(std::pair<std::vector<double>, std::vector<size_t>>(result, {r, c}));
	}
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
    else if (CAN_MAKE(Func*, func)_FROM(expr)) {
	runtime_assert(FUNC_EXISTS(func->func.lexeme), func->func, "Identifier doesn't correspond to a defined function name");
	FuncDecl* funcDecl = func_symbol_table.at(func->func.lexeme);
	Environment env;
	env.func_symbol_table = func_symbol_table;
	env.op_symbol_table = op_symbol_table;
	runtime_assert(func->args.size() == funcDecl->params.size(), func->paren, "Function called with different number of args than defined with");
	for (size_t i = 0; i < func->args.size(); i++) {
	    env.add_var(funcDecl->params.at(i).lexeme, evaluate_expr(func->args.at(i)));
	}
	for (Stmt* stmt: funcDecl->stmts) {
	    env.execute_stmt(stmt);
	}
	return env.get_return_val();
    }
    else if (CAN_MAKE(Literal*, literal)_FROM(expr)) {
	switch (literal->literal_type) {
	case LITERAL_STRING: return Variable(literal->string_val);
	case LITERAL_DOUBLE: return Variable(literal->double_val);
	case LITERAL_BOOL: return Variable(literal->bool_val);
	case LITERAL_ARRAY: {
	    std::vector<double> nums;
	    for (Expr* expr : literal->array_vals) {
		Variable val = evaluate_expr(expr);
		runtime_assert(val.is_double(), literal->token, "Expression in array literal evaluates to a non-number");
		nums.push_back(std::get<double>(val.value));
	    }
	    return Variable(std::pair<std::vector<double>, std::vector<size_t>>(nums, {nums.size()}));
	}
	}
    }
    else if (CAN_MAKE(Unary*, unary)_FROM(expr)) {
	Variable val = evaluate_expr(unary->right);
	switch(unary->op.type) {
	case EXCLA: {
	    runtime_assert(val.is_bool(), unary->op, "Expression evaluates to a non-bool");
	    return Variable(!std::get<bool>(val.value));
	}
	case MINUS: {
	    runtime_assert(val.is_double(), unary->op, "Expression evaluates to a non-number");
	    return Variable(-std::get<double>(val.value));
	}
	case SHAPE: {
	    runtime_assert(val.is_ndarray(), unary->op, "Expression evaluates to a non-ndarray");
	    std::vector<double> casted_shape;
	    for (size_t d : std::get<std::pair<std::vector<double>, std::vector<size_t>>>(val.value).second) {
		casted_shape.push_back((double) d);
	    }
	    return Variable(std::pair<std::vector<double>, std::vector<size_t>>(casted_shape, {casted_shape.size()}));
	}
	default: runtime_assert(false, unary->op, "Invalid unary operator");
	}
    }
    else if (CAN_MAKE(Var*, var)_FROM(expr)) {
	runtime_assert(VAR_EXISTS(var->name.lexeme), var->name, "Identifier doesn't correspond to a declared variable name");
	return var_symbol_table.at(var->name.lexeme);
    }
    else if (CAN_MAKE(Nil*, nil)_FROM(expr)) {
	return Variable();
    }
    throw std::runtime_error("Couldn't evaluate expression (evaluation for expression type might not be implemented?)");
}

void Environment::runtime_assert(bool cond, Token loc, std::string error_msg) {
    if (!cond) throw std::runtime_error(create_error(error_msg, loc));
}

std::string Environment::create_error(std::string error_msg, Token loc) {
    return "Runtime error: " + error_msg + ", occurred at line " + std::to_string(loc.line) + " at column " + std::to_string(loc.col);
}
