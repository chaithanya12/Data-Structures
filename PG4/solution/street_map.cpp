#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cassert>

#include "street_map.hpp"

using namespace std;

street_map::street_map (const string &filename) {
  string line;
  fstream infile(filename);
  string name;
  data.clear();
  while (getline(infile, line)) {
    istringstream iss(line);
    string code;
    iss >> code >> ws;
    if (code == "N:") {
      getline(iss, name);
    } else if (code == "R:") {
      int parity, fromhn, tohn, fromnode, tonode;
      float length;
      string url;
      iss >> parity >> fromhn >> tohn >> fromnode >> tonode >> length >> url;
      side s(name, parity);
      if (data.find(s) == data.end())
	data[s] = vector<segment>();
      data[s].push_back(segment(fromhn, tohn, url));
    } else {
      throw runtime_error("bad file format");
    }
  }
  // The segments are supposed to be sorted, but just in case...
  for (auto side_segment : data)
    sort(side_segment.second.begin(), side_segment.second.end());
}

bool street_map::geocode(const string &address, string &url) const {
  istringstream iss(address);
  int hn;
  string name;
  iss >> hn >> ws;
  getline(iss, name);
  auto pair_it = data.find(side(name, hn % 2));
  if (pair_it == data.end())
    return false;
  auto segs = pair_it->second;
  auto seg_it = lower_bound(segs.begin(), segs.end(), segment(hn, hn, "dummy"));
  if (seg_it == segs.end())
    return false;
  assert (seg_it->tohn >= hn);
  if (seg_it->fromhn > hn)
    return false;
  url = seg_it->url;
  return true;
}
