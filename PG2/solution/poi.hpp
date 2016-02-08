#include <string>

struct poi {
  double lat;
  double lon;
  std::string name;
  poi(double y, double x, const std::string &s) : lat(y), lon(x), name(s) {}
};
