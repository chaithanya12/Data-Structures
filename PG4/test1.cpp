#include <iostream>
#include "street_map.hpp"

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

int main (int argc, char *argv[]) {
  street_map m("southbend.map");
  string url;

  // Test some famous addresses
  test_equal(m.geocode("1417 E Wayne St", url), true);
  test_equal(url, "https://maps.googleapis.com/maps/api/staticmap?maptype=roadmap&size=640x640&path=enc%3Amij%7DFrlhmOL%7DI%40cB");
  
  test_equal(m.geocode("620 W Washington St", url), true);
  test_equal(url, "https://maps.googleapis.com/maps/api/staticmap?maptype=roadmap&size=640x640&path=enc%3A%7D%7Bj%7DF%7EenmO%40fG");

  test_equal(m.geocode("501 W South St", url), true);
  test_equal(url, "https://maps.googleapis.com/maps/api/staticmap?maptype=roadmap&size=640x640&path=enc%3Ayqi%7DF%7EjmmO%40%5E%40%7C%40%3FrBBrF%3Fv%40");

  test_equal(m.geocode("4477 Progress Dr", url), true);
  test_equal(url, "https://maps.googleapis.com/maps/api/staticmap?maptype=roadmap&size=640x640&path=enc%3Aouo%7DFn%7EwmOx%40rH");

  // Test boundary conditions
  test_equal(m.geocode("298 Bercliff Dr", url), false);
  test_equal(m.geocode("299 Bercliff Dr", url), false);
  test_equal(m.geocode("300 Bercliff Dr", url), true);
  test_equal(url, "https://maps.googleapis.com/maps/api/staticmap?maptype=roadmap&size=640x640&path=enc%3Aqmk%7DFdgbmOGbAEl%40GV");
  test_equal(m.geocode("301 Bercliff Dr", url), true);
  test_equal(url, "https://maps.googleapis.com/maps/api/staticmap?maptype=roadmap&size=640x640&path=enc%3Aqmk%7DFdgbmOGbAEl%40GV");

  test_equal(m.geocode("309 Bercliff Dr", url), true);
  test_equal(url, "https://maps.googleapis.com/maps/api/staticmap?maptype=roadmap&size=640x640&path=enc%3Aqmk%7DFdgbmOGbAEl%40GV");
  test_equal(m.geocode("311 Bercliff Dr", url), true);
  test_equal(url, "https://maps.googleapis.com/maps/api/staticmap?maptype=roadmap&size=640x640&path=enc%3Agnk%7DFnkbmOAFQb%40Q%5ESTQRQL");
  test_equal(m.geocode("314 Bercliff Dr", url), true);
  test_equal(url, "https://maps.googleapis.com/maps/api/staticmap?maptype=roadmap&size=640x640&path=enc%3Aqmk%7DFdgbmOGbAEl%40GV");
  test_equal(m.geocode("316 Bercliff Dr", url), true);
  test_equal(url, "https://maps.googleapis.com/maps/api/staticmap?maptype=roadmap&size=640x640&path=enc%3Agnk%7DFnkbmOAFQb%40Q%5ESTQRQL");

  test_equal(m.geocode("398 Bercliff Dr", url), true);
  test_equal(url, "https://maps.googleapis.com/maps/api/staticmap?maptype=roadmap&size=640x640&path=enc%3Aeqk%7DFrobmOCBWHYF%5BB%5B%3F");
  test_equal(m.geocode("399 Bercliff Dr", url), true);
  test_equal(url, "https://maps.googleapis.com/maps/api/staticmap?maptype=roadmap&size=640x640&path=enc%3Aeqk%7DFrobmOCBWHYF%5BB%5B%3F");
  test_equal(m.geocode("400 Bercliff Dr", url), false);
  test_equal(m.geocode("401 Bercliff Dr", url), false);

  test_equal(m.geocode("2416 Roger St", url), true);
  test_equal(url, "https://maps.googleapis.com/maps/api/staticmap?maptype=roadmap&size=640x640&path=enc%3Aobl%7DFfgsmO%40lC");
  test_equal(m.geocode("2418 Roger St", url), false);
  test_equal(m.geocode("2448 Roger St", url), false);
  test_equal(m.geocode("2450 Roger St", url), true);
  test_equal(url, "https://maps.googleapis.com/maps/api/staticmap?maptype=roadmap&size=640x640&path=enc%3Ambl%7DFtksmO%40pC");

  if (failures == 1)
    cout << "One test FAILED" << endl;
  else if (failures)
    cout << failures << " tests FAILED" << endl;
  else
    cout << "All correctness tests PASSED" << endl;
}
