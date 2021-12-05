// This file is part of weak-lang.
// weak-lang is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
// weak-lang is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
// You should have received a copy of the GNU Affero General Public License
// along with weak-lang. If not, see <https://www.gnu.org/licenses/>.

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include <variant>
#include <vector>
#include <string>

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
    std::variant<std::string, bool, double, std::pair<std::vector<double>, std::vector<size_t>>, void*> value;
};

#endif // VARIABLE_H_
