#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <iostream>

#include "street_map.hpp"

using namespace std;

int main (int argc, char *argv[]) {

  // The streets with the most segments
  vector<string> streets = {
    "W Washington St",
    "Lincoln Way W",
    "Ford St",
    "S Michigan St",
    "Huron St",
    "Fellows St",
    "Miami St",
    "W Western Ave",
    "High St",
    "W Mishawaka Ave",
  };

  street_map m("southbend.map");

  default_random_engine g(42);
  uniform_int_distribution<int> ranstreet(0, streets.size()-1), ranhn(1, 9999);
  const int n = 1000000;
  string url;

  cout << "performing " << n << " random queries\n";
  for (int i=0; i<n; i++) {
    ostringstream oss;
    oss << ranhn(g) << " " << streets[ranstreet(g)];
    m.geocode(oss.str(), url);
  }
}
