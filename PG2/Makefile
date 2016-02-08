CXX=~csesoft/new/bin/g++
CC=gcc
CXXFLAGS=-O3 -DNDEBUG -std=c++11

all: distance1 distance2 distance3 distance4a distance4b distance5 measure

distance1: distance1.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

distance2: distance2.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

distance3: distance3.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

distance4a: distance4a.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

distance4b: distance4b.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

distance5: distance5.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

measure: measure.c
	$(CC) $< -o $@

clean:
	rm -f measure distance[1-5] distance[1-5]? *.exe *.o *.error *.log *.output *.submit *.dSYM *~ gmon.out

