#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <unistd.h> // for isatty

#include <vector>
#include "poi.hpp"

using namespace std;

const bool interactive = isatty(0);
int linenum = 0;

void error (const char *s) {
  if (interactive) {
    cout << "error: " << s << endl;
  } else {
    cerr << "error: " << s << " on line " << linenum << endl;
  }
}

template <class C>
int store(C &container, const string &filename) {
  string name, line;
  double lat, lon;
  ifstream base(filename);
  while (getline(base, line)) {
    stringstream sline(line);
    sline >> lat >> lon >> ws;
    getline(sline, name);
    poi *place = new poi(lat, lon, name);
    container.push_back(*place);
  }

  if (container.empty()) {
    error("no points of interest");
    return -1;
  }

  return 0;
}

double calc_dist(const poi me, const poi other) {
    double phi1 = me.lat, lam1 = me.lon;
    double phi2 = other.lat, lam2 = other.lon;
    double scal = 24901 / (2 * M_PI), conv = M_PI / 180;

    double dist = scal * acos((sin(conv * phi1) * sin(conv * phi2)) + \
                (cos(conv * phi1) * cos(conv * phi2) * cos(conv * (lam1 - lam2))));

    return dist;
}

int main (int argc, char *argv[]) {
  if (argc != 4) {
    cout << "Usage: ./distance <base> <latitude> <longitude>" << endl;
    return -1;
  }

  const string filename = (const string)argv[1];
  poi *me = new poi(stod((const string)argv[2]), stod((const string)argv[3]), "HERE");

  vector<poi> container;
  if (store(container, filename)) {
    error("not stored");
    return -1;
  }

  for (vector<poi>::iterator it = container.begin(); it != container.end(); ++it) {
    double dist = calc_dist(*it, *me);
    cout << dist << " " << it->name << endl;
  }

  return 0;
}
