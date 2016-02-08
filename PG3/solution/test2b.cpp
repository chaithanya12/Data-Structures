#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <random>

#include "quadtree.hpp"

using namespace std;

void read_file(const string &filename, quadtree &qt) {
  ifstream base(filename);
  if (!base) {
    cerr << "couldn't read " << filename << endl;
    exit(1);
  }

  string name, line;
  double lat, lon;
  while (getline(base, line)) {
    stringstream sline(line);
    sline >> lat >> lon >> ws;
    getline(sline, name);
    qt.insert(name, point(lat, lon));
  }
}

int main (int argc, char *argv[]) {
  cout << "reading osmpoi.txt" << endl;
  quadtree qt;
  read_file("osmpoi.txt", qt);
  vector<string> result;

  const int n = 100000;
  const double r = 10;
  cout << "Performing " << n << " random queries" << endl;
  default_random_engine g(42);
  uniform_real_distribution<double> ranlat(24, 50), ranlon(-125, -67);
  for (int i=0; i<n; i++) {
    point p(ranlat(g), ranlon(g));
    qt.within_radius(p, r, result);
  }
}
