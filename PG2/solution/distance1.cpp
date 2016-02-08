#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "poi.hpp"

using namespace std;

void error (const char *s) {
  cout << "error: " << s << endl;
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
    poi place(lat, lon, name);
    container.push_back(place);
  }

  if (container.empty()) {
    error("no points of interest");
    return -1;
  }

  return 0;
}

int main (int argc, char *argv[]) {
  if (argc != 4) {
    cout << "Usage: ./distance <database> <latitude> <longitude>" << endl;
    return -1;
  }

  const string filename(argv[1]);
  poi *me = new poi(stod(argv[2]), stod(argv[3]), "Current Location");
  
  vector<poi> container;
  if (store(container, filename)) {
    cout << "could not read database" << endl;
    return -1;
  } else {
    cout << "succesfully read database" << endl;
    return 0;
  }
}
