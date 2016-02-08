CXX=g++
CC=gcc
CXXFLAGS=-std=c++11 -O3

all: test1 test2 geocode measure 

test1: test1.cpp street_map.cpp street_map.hpp
	$(CXX) $(CXXFLAGS) $< street_map.cpp -o $@

test2: test2.cpp street_map.cpp street_map.hpp
	$(CXX) $(CXXFLAGS) $< street_map.cpp -o $@

geocode: geocode.cpp street_map.cpp street_map.hpp
	$(CXX) $(CXXFLAGS) $< street_map.cpp -o $@

measure: measure.c
	$(CC) $< -o $@
clean:
	rm -rf test1 test2 geocode measure *.exe *.o *.dSYM gmon.out *~

