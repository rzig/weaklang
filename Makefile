CXX=clang++
CXXFLAGS=-std=c++20 -g -fstandalone-debug -Wall -Wextra -Werror -pedantic -Iinclude/

weak: bin/weak

bin/weak: bin/main.o bin/lexer.o
	$(CXX) $(CXXFLAGS) $^ -o $@
bin/main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@
bin/lexer.o: src/lexer.cpp include/lexer.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

.DEFAULT_GOAL := weak
.PHONY: clean weak

clean:
	rm -rf bin/*
