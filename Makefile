CXX=clang++
CXXFLAGS=-std=c++20 -g -fstandalone-debug -Iinclude/
LFLAGS=-lcblas

weak: bin/weak
tests: bin/tests

bin/weak: bin/main.o bin/lexer.o bin/error.o bin/stmt.o bin/token.o bin/expr.o bin/parser.o bin/environment.o bin/variable.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LFLAGS)
bin/main.o: src/main.cpp include/lexer.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@
bin/lexer.o: src/lexer.cpp include/lexer.hpp include/token.hpp include/error.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@
bin/error.o: src/error.cpp include/error.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@
bin/util.o: src/util.cpp include/util.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@
bin/token.o: src/token.cpp include/token.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@
bin/stmt.o: src/stmt.cpp include/stmt.hpp include/token.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@
bin/expr.o: src/expr.cpp include/expr.hpp include/token.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@
bin/parser.o: src/parser.cpp include/parser.hpp include/token.hpp include/stmt.hpp include/expr.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@
bin/environment.o: src/environment.cpp include/environment.hpp include/variable.hpp include/parser.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@
bin/variable.o: src/variable.cpp include/variable.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

bin/tests: bin/catch.o tests/tests.cc src/lexer.cpp src/token.cpp src/error.cpp src/stmt.cpp src/expr.cpp src/parser.cpp src/util.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

bin/catch.o: tests/catch.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@

.DEFAULT_GOAL := weak
.PHONY: clean weak

clean:
	rm -rf bin/*
