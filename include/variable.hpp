#ifndef VARIABLE_H_
#define VARIABLE_H_

#include <variant>
#include <vector>

class Variable {
public:
    Variable();
    Variable(std::string var);
    Variable(bool var);
    Variable(double var);
    Variable(std::pair<std::vector<double>, std::vector<size_t>> var);
    ~Variable() = default;
    bool is_string();
    bool is_bool();
    bool is_double();
    bool is_ndarray();
    bool is_nil();
private:
    std::variant<std::string, bool, double, std::pair<std::vector<double>, std::vector<size_t>>, void*> value;
};

#endif // VARIABLE_H_
