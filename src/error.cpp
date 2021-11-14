#include "error.hpp"

Error::Error(std::string msg, size_t l, size_t c) : message(msg), line(l), column(c) {};