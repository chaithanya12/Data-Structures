#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <random>

#include "quadtree.hpp"

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

  cout << "reading ndfood.txt" << endl;
  quadtree qt;
  read_file("ndfood.txt", qt);
  vector<string> result;

  cout << "Testing within_radius" << endl;
  cout << "qt.within_radius(point(41.7031036, -86.2391732), 0.2, result);" << endl;
  qt.within_radius(point(41.7031036, -86.2391732), 0.2, result);
  sort(result.begin(), result.end());
  test_equal(result.size(), 3);
  if (result.size() >= 1) test_equal(result[0], "Cafe Poche (Bond Hall)");
  if (result.size() >= 2) test_equal(result[1], "LaFortune Student Center");
  if (result.size() >= 3) test_equal(result[2], "North Dining Hall");

  cout << "qt.within_radius(point(41.699476, -86.237183), 0.2, result);" << endl;
  qt.within_radius(point(41.699476, -86.237183), 0.2, result);
  sort(result.begin(), result.end());
  test_equal(result.size(), 8);
  if (result.size() >= 1) test_equal(result[0], "Cafe Commons (Mendoza College of Business)");
  if (result.size() >= 2) test_equal(result[1], "Crossings (Eck Hall of Law)");
  if (result.size() >= 3) test_equal(result[2], "Decio Commons");

  cout << "qt.within_radius(point(41.704886, -86.236876), 0.2, result);" << endl;
  qt.within_radius(point(41.704886, -86.236876), 0.2, result);
  sort(result.begin(), result.end());
  test_equal(result.size(), 2);
  if (result.size() >= 1) test_equal(result[0], "Cafe de Grasta (Grace Hall)");
  if (result.size() >= 2) test_equal(result[1], "North Dining Hall");

  cout << "qt.within_radius(point(41.7031036, -86.2391732), 0.1, result);" << endl;
  qt.within_radius(point(41.7031036, -86.2391732), 1.0, result);
  sort(result.begin(), result.end());
  test_equal(result.size(), 17);

  cout << "qt.within_radius(point(41.7031036, -86.2391732), 0.01, result);" << endl;
  qt.within_radius(point(41.7031036, -86.2391732), 0.01, result);
  sort(result.begin(), result.end());
  test_equal(result.size(), 0);

  cout << endl;

  cout << "Testing copy constructor" << endl;
  quadtree qt1(qt);
  cout << "qt1.within_radius(point(41.7031036, -86.2391732), 0.2, result);" << endl;
  qt1.within_radius(point(41.7031036, -86.2391732), 0.2, result);
  sort(result.begin(), result.end());
  test_equal(result.size(), 3);
  if (result.size() >= 1) test_equal(result[0], "Cafe Poche (Bond Hall)");
  if (result.size() >= 2) test_equal(result[1], "LaFortune Student Center");
  if (result.size() >= 3) test_equal(result[2], "North Dining Hall");

  cout << "Testing copy assignment" << endl;
  qt = qt1;
  cout << "qt1.within_radius(point(41.7031036, -86.2391732), 0.2, result);" << endl;
  qt1.within_radius(point(41.7031036, -86.2391732), 0.2, result);
  sort(result.begin(), result.end());
  test_equal(result.size(), 3);
  if (result.size() >= 1) test_equal(result[0], "Cafe Poche (Bond Hall)");
  if (result.size() >= 2) test_equal(result[1], "LaFortune Student Center");
  if (result.size() >= 3) test_equal(result[2], "North Dining Hall");

  cout << endl;

  if (failures == 1)
    cout << "One test did not pass." << endl;
  else if (failures)
    cout << failures << " tests did not pass." << endl;
  else
    cout << "All tests passed!" << endl;
}
