#ifndef STREET_MAP_HPP
#define STREET_MAP_HPP

#include <vector>
#include <unordered_map>
#include <boost/functional/hash.hpp>
using namespace std;


typedef struct my_side {
	int parity;
	string street_name;
	my_side(int p, string sn): parity(p), street_name(sn) {};
	bool operator==(struct my_side const& other) const { return parity==other.parity && street_name==other.street_name; }
} side;

inline size_t hash_value(side const& s){
	size_t h = boost::hash<string>()(s.street_name);
	boost::hash_combine(h,s.parity);
	return h;
}


class street_map {

public:
	// You need to write implementations of these functions in street_map.cpp.

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
	// Add any other member variables and functions you want.

	// The main data structure must have a type of the form:
	//   std::unordered_map<side, ...> 
	typedef struct my_segment {
		int start_address, end_address;
		string url;
		my_segment(int sa, int ea, string u): start_address(sa), end_address(ea), url(u) {};
	} segment;

	unordered_map<side,vector<segment>,boost::hash<side>> address_table;
};

#endif
