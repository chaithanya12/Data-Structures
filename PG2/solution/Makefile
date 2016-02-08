CXXFLAGS=-std=c++11

all: distance1 distance2 distance3 distance4a distance4b distance5m distance5q

distance1: distance1.cpp
	g++ $(CXXFLAGS) $^ -o $@
distance2: distance2.cpp
	g++ $(CXXFLAGS) $^ -o $@
distance3: distance3.cpp
	g++ $(CXXFLAGS) $^ -o $@
distance4a: distance4a.cpp
	g++ $(CXXFLAGS) $^ -o $@
distance4b: distance4b.cpp
	g++ $(CXXFLAGS) $^ -o $@
distance5q: distance5q.cpp
	g++ $(CXXFLAGS) $^ -o $@
distance5m: distance5m.cpp
	g++ $(CXXFLAGS) $^ -o $@

clean:
	rm -rf *.dSYM *~ *.exe distance[123] distance4[ab] distance5[qm]