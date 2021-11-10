CXX=clang++
CXXFLAGS=-std=c++20 -g -fstandalone-debug -Wall -Wextra -Werror -pedantic -Iinclude/

weak: bin/weak
tests: bin/tests

bin/weak: bin/main.o bin/lexer.o bin/token.o
	$(CXX) $(CXXFLAGS) $^ -o $@
bin/main.o: src/main.cpp include/lexer.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@
bin/lexer.o: src/lexer.cpp include/lexer.hpp include/token.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@
bin/token.o: src/token.cpp include/token.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

bin/tests: bin/catch.o tests/tests.cc src/lexer.cpp src/token.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

bin/catch.o: tests/catch.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@

.DEFAULT_GOAL := weak
.PHONY: clean weak

clean:
	rm -rf bin/*
