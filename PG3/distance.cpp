#include <cmath>
#include <algorithm>
#include "distance.hpp"

using namespace std;

double clip(double x, double lo, double hi) { 
  // Clips x to the interval [lo, hi].
  return max(min(x, hi), lo); 
}

const double deg_to_rad = M_PI / 180;
const double rad_to_mi = 24901 / (2 * M_PI);

double distance(const point &p1, const point &p2) {
  // Finds the great-circle distance between p1 and p2.
  double lat1 = deg_to_rad * p1.lat;
  double lat2 = deg_to_rad * p2.lat;
  double dlon = deg_to_rad * (p1.lon - p2.lon); // sign doesn't matter
  return rad_to_mi * acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(dlon));
}

double distance_to_box(const point &p, const point &sw, const point &ne) {
  // Finds the minimum distance from p to the box whose southwest
  // corner is sw and northeast corner is ne.
  // Preconditions:
  // - sw.lat <= ne.lat
  // - sw.lon <= ne.lon (Note that this means that the box cannot cross longitude 180.)
  point nearest;
  nearest.lon = clip(p.lon, sw.lon, ne.lon);
  double lat = deg_to_rad * p.lat;
  double dlon = deg_to_rad * (p.lon-nearest.lon); // sign doesn't matter
  nearest.lat = clip(atan2(sin(lat), cos(lat) * cos(dlon)) / deg_to_rad, sw.lat, ne.lat);
  return distance(p, nearest);
}
