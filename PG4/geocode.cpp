#include "street_map.hpp"

#include <iostream>
#include <sstream>

using namespace std;

int main (int argc, char *argv[]) {
  street_map m("southbend.map");
  string addr, url;

  ostringstream oss;
  for (int i=1; i<argc; i++) {
    if (i>1)
      oss << " ";
    oss << argv[i];
  }
  addr = oss.str();
  if (m.geocode(addr, url))
    cout << url << endl;
  else
    cout << "Address not found.\n";
}
