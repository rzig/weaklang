#include "variable.hpp"

Variable() {
    value.emplace<4>(nullptr);
}

Variable(std::string var) {
    value.emplace<0>(var);
}

Variable(bool var) {
    value.emplace<1>(var);
}

Variable(double var) {
    value.emplace<2>(var);
}

Variable(std::pair<std::vector<double>, std::vector<size_t>> var) {
    value.emplace<3>(var);
}

~Variable() = default;

bool is_string() {
    return std::get_if<std::string>(&value);
}

bool is_bool() {
    return std::get_if<bool>(&value);
}

bool is_double() {
    return std::get_if<double>(&value);
}

bool is_ndarray() {
    return std::get_if<std::pair<std::vector<double>, std::vector<size_t>>>(&value);
}

bool is_nil() {
    return std::get_if<void*>(&value);
}
