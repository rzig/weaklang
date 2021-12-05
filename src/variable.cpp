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

#include "variable.hpp"

Variable::Variable() {
    value.emplace<4>(nullptr);
}

Variable::Variable(std::string var) {
    value.emplace<0>(var);
}

Variable::Variable(bool var) {
    value.emplace<1>(var);
}

Variable::Variable(double var) {
    value.emplace<2>(var);
}

Variable::Variable(std::pair<std::vector<double>, std::vector<size_t>> var) {
    value.emplace<3>(var);
}

bool Variable::is_string() {
    return std::get_if<std::string>(&value);
}

bool Variable::is_bool() {
    return std::get_if<bool>(&value);
}

bool Variable::is_double() {
    return std::get_if<double>(&value);
}

bool Variable::is_ndarray() {
    return std::get_if<std::pair<std::vector<double>, std::vector<size_t>>>(&value);
}

bool Variable::is_nil() {
    return std::get_if<void*>(&value);
}
