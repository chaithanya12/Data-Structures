#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <random>

#include "bintree.hpp"

using namespace std;

int failures = 0;

#define test_equal(x, y) do { \
  cout << "testing " << #x << " == " << #y << ": "; \
  if (x == y) \
    cout << "PASSED" << endl; \
  else { \
    cout << "FAILED (" << #x << " was " << x << ")" << endl; \
    failures++; \
  } \
} while (0)

#define test_close(x, y) do { \
  cout << "testing " << #x << " == " << #y << ": "; \
  if (fabs(x-y) <= 1e-3) \
    cout << "PASSED" << endl; \
  else { \
    cout << "FAILED (" << #x << " was " << x << ")" << endl; \
    failures++; \
  } \
} while (0)

void read_file(const string &filename, bintree &bt) {
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
    bt.insert(name, lon);
  }
}

int main (int argc, char *argv[]) {

  cout << "reading ndfood.txt" << endl;
  bintree bt;
  read_file("ndfood.txt", bt);
  vector<string> result;

  cout << "Testing within_radius" << endl;
  cout << "bt.within_radius(-86.2391732, 0.001, result);" << endl;
  bt.within_radius(-86.2391732, 0.001, result);
  sort(result.begin(), result.end());
  test_equal(result.size(), 2);
  if (result.size() >= 1) test_equal(result[0], "Rohr's (Morris Inn)");
  if (result.size() >= 2) test_equal(result[1], "Sorin's (Morris Inn)");

  cout << "bt.within_radius(-86.237183, 0.001, result);" << endl;
  bt.within_radius(-86.237183, 0.001, result);
  sort(result.begin(), result.end());
  test_equal(result.size(), 4);
  if (result.size() >= 1) test_equal(result[0], "Au Bon Pain (Hesburgh Center for International Studies)");
  if (result.size() >= 2) test_equal(result[1], "Crossings (Eck Hall of Law)");
  if (result.size() >= 3) test_equal(result[2], "Kitz Kafe (Stinson-Remick Hall of Engineering)");
  if (result.size() >= 4) test_equal(result[3], "LaFortune Student Center");

  cout << "bt.within_radius(-86.233158, 0.001, result);" << endl;
  bt.within_radius(-86.233158, 0.001, result);
  sort(result.begin(), result.end());
  test_equal(result.size(), 1);
  if (result.size() >= 1) test_equal(result[0], "Cafe de Grasta (Grace Hall)");

  cout << "bt.within_radius(-86.2391732, 0.01, result);" << endl;
  bt.within_radius(-86.2391732, 0.01, result);
  sort(result.begin(), result.end());
  test_equal(result.size(), 17);

  cout << "bt.within_radius(-86.2391732, 0.00001, result);" << endl;
  bt.within_radius(-86.2391732, 0.00001, result);
  sort(result.begin(), result.end());
  test_equal(result.size(), 0);

  cout << endl;

  cout << "Testing copy constructor" << endl;
  bintree bt1(bt);
  cout << "bt1.within_radius(-86.2391732, 0.001, result);" << endl;
  bt1.within_radius(-86.2391732, 0.001, result);
  sort(result.begin(), result.end());
  test_equal(result.size(), 2);
  if (result.size() >= 1) test_equal(result[0], "Rohr's (Morris Inn)");
  if (result.size() >= 2) test_equal(result[1], "Sorin's (Morris Inn)");

  cout << "Testing copy assignment" << endl;
  bintree bt2;
  bt2 = bt1;
  cout << "bt2.within_radius(-86.2391732, 0.001, result);" << endl;
  bt2.within_radius(-86.2391732, 0.001, result);
  sort(result.begin(), result.end());
  test_equal(result.size(), 2);
  if (result.size() >= 1) test_equal(result[0], "Rohr's (Morris Inn)");
  if (result.size() >= 2) test_equal(result[1], "Sorin's (Morris Inn)");

  cout << endl;

  if (failures == 1)
    cout << "One test did not pass." << endl;
  else if (failures)
    cout << failures << " tests did not pass." << endl;
  else
    cout << "All tests passed!" << endl;
}
