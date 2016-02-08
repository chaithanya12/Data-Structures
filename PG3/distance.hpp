#ifndef DISTANCE_HPP
#define DISTANCE_HPP

struct point {
  double lat, lon; // Measured in degrees.
  point() : lat(0.), lon(0.) { }
  point(double lat, double lon) : lat(lat), lon(lon) { }
  point(const point& p) : lat(p.lat), lon(p.lon) { }
};

// Finds the great-circle distance between p1 and p2.
double distance(const point &p1, const point &p2);

// Finds the minimum distance from p to the box whose southwest corner
// is sw and northeast corner is ne.
double distance_to_box(const point &p, const point &sw, const point &ne);

#endif
