#include <iostream>
#include <fstream>
#include <sstream>
#include "street_map.hpp"

using namespace std;

street_map::street_map (const string &filename) {
	// Open file filename
	ifstream in_file;
	in_file.open(filename.c_str(),ios::in);
	// If the file does not exist, throw error and exit program
	if(! in_file){
		cerr << "Error: Filename \"" << filename << "\" does not exist in this directory." << endl;
		exit(1);
	}
	// read in the file
	string newline, next_word, street_name;
	while( getline(in_file,newline) ){
		string url;
		int parity, start_address, end_address, garbage_int;
		float garbage_float;
		istringstream iss(newline);
		// read first word to see if line is a N name or R range
		iss>>next_word;

		if( next_word=="N:" ) {
			// new street data incoming
			street_name.clear();
			// read remaining words in line
			while( iss>>next_word ){
				street_name += (next_word+" ");
			}
			// pop last space
			street_name.pop_back();
			// insert two pairs into the map - one for each parity of the new street
			address_table.insert( 
				make_pair<side,vector<segment>>( side(0,street_name), vector<segment>() )
			);
			address_table.insert( 
				make_pair<side,vector<segment>>( side(1,street_name), vector<segment>() )
			);
		} else {
			// read remaining words in line
			iss>>parity>>start_address>>end_address
				>>garbage_int>>garbage_int>>garbage_float
				>>url;
			// insert new segment into correct map value vector
			address_table[side(parity,street_name)].push_back(segment(start_address,end_address,url));
		}
	}
	// close file connection
	in_file.close();
}

bool street_map::geocode(const string &address, string &url) const {

	string next_word, street_name;
	int add_num;
	istringstream iss(address);
	// first 'word' of address is the number
	iss>>add_num;
	while(iss>>next_word){
		street_name += (next_word+" ");
	}
	// pop trailing space char
	street_name.pop_back();
	// find the correct map key-value pair for the given address
	unordered_map<side,vector<segment>,boost::hash<side>>::const_iterator map_pair = address_table.find(side(add_num%2,street_name));
	// loop through vector of segments on the corresponding side to find the correct range
	for (vector<segment>::const_iterator it=(map_pair->second).begin(); it!=(map_pair->second).end(); ++it){
		if( it->start_address<=add_num && it->end_address>=add_num ){
			url = it->url;
			return true;
		}
	}
	return false;
}
