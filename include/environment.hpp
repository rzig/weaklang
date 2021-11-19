#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <unordered_map>
#include <string>

#include "parser.hpp"

class Environment {
public:

private:
    std::unordered_map<std::string, Expr> values; 
};

#endif // ENVIRONMENT_H_