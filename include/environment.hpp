#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <unordered_map>
#include <stdexcept>
#include <string>
#include <math.h>

#include "variable.hpp"
#include "parser.hpp"
#include "error.hpp"
#include "util.hpp"

#define FUNC_EXISTS(func) (func_symbol_table.find(func) != func_symbol_table.end())
#define OP_EXISTS(op) (op_symbol_table.find(op) != op_symbol_table.end())
#define VAR_EXISTS(var) (var_symbol_table.find(var) != var_symbol_table.end())

#define ELEMENTWISE_OP(OP) { \
    if (left_var.is_double() && right_var.is_double()) { \
	return Variable(std::get<double>(left_var.value) OP std::get<double>(right_var.value)); \
    } \
    if (left_var.is_double() && right_var.is_ndarray()) { \
	auto right_arr = std::get<std::pair<std::vector<double>, std::vector<size_t>>>(right_var.value).first; \
	for (size_t i = 0; i < right_arr.size(); i++) { \
	    right_arr.at(i) = std::get<double>(left_var.value) OP right_arr.at(i); \
	} \
	return Variable(std::pair<std::vector<double>, std::vector<size_t>>(right_arr, std::get<std::pair<std::vector<double>, std::vector<size_t>>>(right_var.value).second)); \
    } \
    if (left_var.is_ndarray() && right_var.is_double()) { \
	auto left_arr = std::get<std::pair<std::vector<double>, std::vector<size_t>>>(left_var.value).first; \
	for (size_t i = 0; i < left_arr.size(); i++) { \
	    left_arr.at(i) = left_arr.at(i) OP std::get<double>(right_var.value); \
	} \
	return Variable(std::pair<std::vector<double>, std::vector<size_t>>(left_arr, std::get<std::pair<std::vector<double>, std::vector<size_t>>>(left_var.value).second)); \
    } \
    if (left_var.is_ndarray() && right_var.is_ndarray()) { \
	auto left_var_pair = std::get<std::pair<std::vector<double>, std::vector<size_t>>>(left_var.value); \
	auto right_var_pair = std::get<std::pair<std::vector<double>, std::vector<size_t>>>(right_var.value); \
	runtime_assert(left_var_pair.second == right_var_pair.second, binary->op, "Expressions evaluate to arrays of differing sizes"); \
	std::vector<double> zipped; \
	for (size_t i = 0; i < left_var_pair.first.size(); i++) { \
	    zipped.push_back(left_var_pair.first.at(i) OP right_var_pair.first.at(i)); \
	} \
	return Variable(std::pair<std::vector<double>, std::vector<size_t>>(zipped, left_var_pair.second)); \
    } \
}

class Environment {
public:
    Environment();
    ~Environment() = default;
    void add_func(std::string name, FuncDecl* func);
    void add_op(std::string name, OpDecl* op);
    void add_var(std::string name, Variable var);
    bool has_hit_return();
    Variable get_return_val();
    void execute_stmt(Stmt* stmt);
    std::unordered_map<std::string, FuncDecl*> func_symbol_table; 
    std::unordered_map<std::string, OpDecl*> op_symbol_table; 
    std::unordered_map<std::string, Variable> var_symbol_table;
private:
    bool hit_return;
    Variable return_val;
    Variable evaluate_expr(Expr* expr);
    void runtime_assert(bool cond, Token loc, std::string error_msg);
    std::string create_error(std::string error_msg, Token loc);
};

#endif // ENVIRONMENT_H_
