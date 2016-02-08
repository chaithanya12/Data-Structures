CXX=~csesoft/new/bin/g++
CC=gcc
CXXFLAGS=-std=c++11 -O3 -DNDEBUG

all: calc0 calc1 calc2 calc3 calc4 measure

calc0: calc0.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

calc1: calc1.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

calc2: calc2.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

calc3: calc3.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

calc4: calc4.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

measure: measure.c
	$(CC) $< -o $@

clean:
	rm -rf calc[0-4] measure *.exe *.o *.dSYM *~ gmon.out

