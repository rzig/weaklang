#ifndef ERROR_H_
#define ERROR_H_

#include<string>

struct Error {
    const std::string message;
    const size_t line;
    const size_t column;

    Error(std::string msg, size_t l, size_t c);
};

#endif