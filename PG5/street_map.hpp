#ifndef STREET_MAP_HPP
#define STREET_MAP_HPP

#include <string>
#include <vector>
#include <utility>
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

typedef struct my_edge {
	string street_name;
	float length;
	int neighbor_node, prev_hop;
	my_edge(string sn, float l, int nn): street_name(sn), length(l), neighbor_node(nn) {};
	my_edge(string sn, float l, int nn, int ph): street_name(sn), length(l), neighbor_node(nn), prev_hop(ph) {};
	bool operator<(struct my_edge const& other) const { return length > other.length; }
} edge;


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
	// Output arguments:
	//   `u` is the starting node of the edge on which the location lies
	//   `v` is the ending node of the edge
	//   `pos` is how far along the edge the location lies

	bool geocode(const std::string &address, int &u, int &v, float &pos) const;

	// Three functions for finding the shortest route.

	// Input arguments:
	//   `source` is the source node
	//   `target` is the target node
	// Output argument:
	//   `distance` is the shortest distance between them

	bool route3(int source, int target, float &distance) const;

	// Input arguments:
	//   `su`, `sv` is the edge on which the source lies
	//   `spos`     is how far along the edge the source lies
	//   `tu`, `tv` is the edge on which the source lies
	//   `tpos`     is how far along the edge the source lies
	// Output argument:
	//   `distance` is the shortest distance between them

	bool route4(int su, int sv, float spos, int tu, int tv, float tpos, float &distance) const;

	// Input arguments:
	//   `su`, `sv` is the edge on which the source lies
	//   `spos`     is how far along the edge the source lies
	//   `tu`, `tv` is the edge on which the source lies
	//   `tpos`     is how far along the edge the source lies
	// Output argument:
	//   `steps`    is a sequence of driving directions;
	//              each member is a canonical street name and a distance.
	//              The original contents (if any) are cleared out.

	bool route(int su, int sv, float spos, int tu, int tv, float tpos, std::vector<std::pair<std::string, float>> &steps) const;

private:
	typedef struct my_segment {
		int start_address, end_address
			, start_node, end_node;
		float length;
		my_segment(int sa, int ea, int sn, int en, float l): start_address(sa), end_address(ea), start_node(sn), end_node(en), length(l) {};
	} segment;

	unordered_map<side,vector<segment>,boost::hash<side>> address_table;
	unordered_map<int,vector<edge>> adjacency_list; // the undirected graph of all edges in map of south bend (or location contents of textfile provided)
};

#endif
