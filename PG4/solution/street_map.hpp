#ifndef STREET_MAP_HPP
#define STREET_MAP_HPP

#include <unordered_map>
#include <string>
#include <vector>

struct side {
  std::string name;
  int parity;
  bool operator==(const side &other) const { return name == other.name && parity == other.parity; }
  side(const std::string &name, int parity) : name(name), parity(parity) { }
};

namespace std {
  template<>
  struct hash<side> {
    hash<string> string_hasher;
    size_t operator()(const side &s) const {
      return string_hasher(s.name) ^ s.parity;
    }
  };
}

struct segment {
  int fromhn, tohn;
  std::string url;
  segment(int fromhn, int tohn, const std::string &url) : fromhn(fromhn), tohn(tohn), url(url) { }
  // Since we assume that segments never overlap,
  // there are lots of equivalent ways of comparing segments.
  bool operator<(const segment &other) const { return tohn < other.tohn; }
};

class street_map {
public:
  // Constructor.
  // `filename` is the pathname of a file with the format described in pg4.pdf.

  explicit street_map (const std::string &filename);

  // Geocodes an address.
  // Input argument:
  //   `address` is an address of the form "1417 E Wayne St"
  // Return value:
  //   `true` if address is found, `false` if address is not found
  // Output argument:
  //   `url` is the URL of an image displaying the street segment
  //   containing the address.

  bool geocode(const std::string &address, std::string &url) const;

private:
  std::unordered_map<side, std::vector<segment>> data;
};

#endif
