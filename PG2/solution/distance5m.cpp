#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h> // for isatty

#include <cmath>
#include <list>
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

struct dist_comp {
    poi me;
    dist_comp(poi p) : me(p) {}
    bool operator () (poi other1, poi other2) const {
      double d1 = calc_dist(me, other1);
      double d2 = calc_dist(me, other2);
      return (d1 < d2 || d1 == d2 && other1.name < other2.name);
    }
};

template <typename Less>
list<poi> domerge(list<poi> &left, list<poi> &right, const Less &less) {
  int i = 0, j = 0;
  int p = left.size(), q = right.size();
  list<poi> l;

  while (i < p && j < q) {
    if (less(left.front(), right.front())) {
      l.push_back(left.front());
      left.pop_front();
      i++;
    } else {
      l.push_back(right.front());
      right.pop_front();
      j++;
    }
  }

  while (i < p) {
      l.push_back(left.front());
      left.pop_front();
      i++;
  }

  while (j < q) {
      l.push_back(right.front());
      right.pop_front();
      j++;
  }

  return l;
}

template <typename Less>
void mergesort(list<poi> &l, const Less &less) {
  if (l.size() > 1) {
    int r = l.size() / 2, k;
    
    list<poi> left;
    for (k = 0; k < r; k++) {
      left.push_back(l.front());
      l.pop_front();
    }

    list<poi> right;
    r = l.size();
    for (k = 0; k < r; k++) {
      right.push_back(l.front());
      l.pop_front();
    }

    mergesort(left, less);
    mergesort(right, less);

    list<poi> s = domerge(left, right, less);
    r = s.size();
    for (k = 0; k < r; k++) {
        l.push_back(s.front());
        s.pop_front();
    }
  }
}

int main (int argc, char *argv[]) {
  if (argc != 4) {
    cout << "Usage: ./distance <base> <latitude> <longitude>" << endl;
    return -1;
  }

  const string filename = (const string)argv[1];
  poi *me = new poi(stod((const string)argv[2]), stod((const string)argv[3]), "HERE");

  list<poi> container;
  if (store(container, filename)) {
    error("not stored");
    return -1;
  }

  mergesort(container, dist_comp(*me));
  for (list<poi>::iterator it = container.begin(); it != container.end(); ++it) {
    double dist = calc_dist(*me, *it);
    cout << dist << " " << it->name << endl;
  }

  return 0;
}
