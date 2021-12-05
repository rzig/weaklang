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
