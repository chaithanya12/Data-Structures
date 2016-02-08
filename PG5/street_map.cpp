/* Author: Patrick Drumm
 * Course: cse30331
 * Program: street_map.cpp (PG5)
 *
 * This program provides the functionality to generate driving directions from one address to another!
 *
 * For general use (after using the Makefile to compile):
 * 	> ./route
 *	> Starting address:
 *		enter a source address, then enter
 *	> Ending address:
 *		enter a destination address, then enter
 *
 *	Output will then show how many miles you must drive on each road (in sequential order) to get to your destination address
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <unordered_set>
#include <algorithm>
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
		int parity, start_address, end_address, start_node, end_node;
		float length;
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
		} else if( next_word=="R:" ){
			// read remaining words in line
			iss>>parity>>start_address>>end_address
				>>start_node>>end_node>>length;

			// insert new segment into correct map value vector in address_table
			address_table[side(parity,street_name)].push_back(segment(start_address,end_address,start_node,end_node,length));

			// Insert new edge pairs into adjacency_list (the undirected graph)
			// account for redundancy: if the node w/in the edge already exists, then update the street name with the new one (the canonical name which occurs last in the file)
			//
			// Insert <start_node,edge(end_node)>
			vector<edge> node_edges = adjacency_list[start_node];
			vector<edge>::iterator it;
			for( it=node_edges.begin(); it!=node_edges.end(); ++it){
				if( it->neighbor_node==end_node && it->length==length ){
					it->street_name = street_name;
					break;
				}
			}
			if( it==node_edges.end() ){ // ie. if the edge was not found to be a duplicate
				adjacency_list[start_node].push_back( edge(street_name,length,end_node) );
			}
			node_edges.clear();

			// Insert <end_node,edge(start_node)>
			node_edges = adjacency_list[end_node];
			for( it=node_edges.begin(); it!=node_edges.end(); ++it){
				if( it->neighbor_node==start_node && it->length==length ){
					it->street_name = street_name;
					break;
				}
			}
			if( it==node_edges.end() ){ // ie. if the edge was not found to be a duplicate
				adjacency_list[end_node].push_back( edge(street_name,length,start_node) );
			}
		} else {
			// its an E:(edge)
			// read remaining words in line
			iss>>start_node>>end_node>>length;

			// Insert <start_node,edge(end_node)>
			vector<edge> node_edges = adjacency_list[start_node];
			vector<edge>::iterator it;
			for( it=node_edges.begin(); it!=node_edges.end(); ++it){
				if( it->neighbor_node==end_node && it->length==length ){
					it->street_name = street_name;
					break;
				}
			}
			if( it==node_edges.end() ){ // ie. if the edge was not found to be a duplicate
				adjacency_list[start_node].push_back( edge(street_name,length,end_node) );
			}
			node_edges.clear();

			// Insert <end_node,edge(start_node)>
			node_edges = adjacency_list[end_node];
			for( it=node_edges.begin(); it!=node_edges.end(); ++it){
				if( it->neighbor_node==start_node && it->length==length ){
					it->street_name = street_name;
					break;
				}
			}
			if( it==node_edges.end() ){ // ie. if the edge was not found to be a duplicate
				adjacency_list[end_node].push_back( edge(street_name,length,start_node) );
			}

		}
	}
	// close file connection
	in_file.close();
}

bool street_map::geocode(const string &address, int &u, int &v, float &pos) const {

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
			// output starting and ending nodes
			u = it->start_node;
			v = it->end_node;
			//calculate pos
			pos = it->length * (add_num - it->start_address)/(it->end_address - it->start_address + 2);

			return true;
		}
	}
	return false;
}

// take two node IDs (source & target) and find the shortest distance between them using Dijkstra's algorithm
bool street_map::route3(int source, int target, float &distance) const {
	// Perform Dijkstra's algorithm using a priority queue
	priority_queue<edge,vector<edge>,less<edge> > frontier;
	unordered_set<int> marked;
	// Push source node with distance of zero
	frontier.push(edge("",0,source));
	while(!frontier.empty()){
		edge next = frontier.top();
		frontier.pop();
		// make sure that the node popped off frontier is not already marked
		if( marked.count(next.neighbor_node)==0 ){
			marked.insert(next.neighbor_node);
			if( next.neighbor_node == target ){
				// if we pop the target off of the priority queue, then we know it is the minimum distance!
				distance = next.length;
				return true; 
			}
			// push all of the neighbor's of the current node into the frontier
			unordered_map<int,vector<edge>>::const_iterator map_pair = adjacency_list.find(next.neighbor_node);
			for (vector<edge>::const_iterator v=(map_pair->second).begin(); v!=(map_pair->second).end(); ++v){
				// don't push node onto frontier if it's marked
				if( marked.count(v->neighbor_node)==0 ){
					frontier.push(edge(v->street_name,v->length+next.length,v->neighbor_node));
				}
			}
		}
	}
	return false;
}

bool street_map::route4(int su, int sv, float spos, int tu, int tv, float tpos, float &distance) const {

	// use a modified dijkstra's algorithm
	float s1, t1;			// length of edge (su,sv), (tu,tv)
	string s_street, t_street;	// street name that (su,sv) and (tu,tv) are on

	// calculate lengths of street segments that s and t lie on
	// calculate length of (su,sv)
	unordered_map<int,vector<edge>>::const_iterator map_pair = adjacency_list.find(su);
	vector<edge>::const_iterator it;
	for (it=(map_pair->second).begin(); it!=(map_pair->second).end(); ++it){
		if( it->neighbor_node==sv ) break;
	}
	if( it==(map_pair->second).end() ) return false;//b/c the node pair does not exist in adjacency list
	s1 = it->length;
	s_street = it->street_name;
	// calculate length of (tu,tv)
	map_pair = adjacency_list.find(tu);
	for (it=(map_pair->second).begin(); it!=(map_pair->second).end(); ++it){
		if( it->neighbor_node==tv ) break;
	}
	if( it==(map_pair->second).end() ) return false;//b/c the node pair does not exist in adjacency list
	t1 = it->length;
	t_street = it->street_name;

	// define arbitrary nodes
	int SOURCE=-2, TARGET=-1;

	// handle case where SOURCE and TARGET lie on same segment
	if( su==tu && sv==tv ){
		distance = abs( spos-tpos );
		return true;
	}
	if( su==tv && sv==tu ){
		distance = abs( s1-(spos+tpos) );
		return true;
	}

	// begin modified dijkstra's
	priority_queue<edge,vector<edge>,less<edge> > frontier;
	unordered_set<int> marked;
	// Push source node with distance of zero, along with two adjacent nodes
	frontier.push(edge("",0,SOURCE));
	frontier.push(edge("",spos,su));
	frontier.push(edge("",s1-spos,sv));
	while(!frontier.empty()){
		edge next = frontier.top();
		int u = next.neighbor_node;
		frontier.pop();
		// make sure that the node popped off frontier is not already marked
		if( marked.count(u)==0 ){
			// take care of special cases where TARGET is a neighbor (b/c TARGET is not actually a node in adjacency_list)
			marked.insert(u);
			if( u == tu ){
				frontier.push(edge(t_street,next.length+tpos,TARGET));
			}
			if( u == tv ){
				frontier.push(edge(t_street,abs(next.length+t1-tpos),TARGET));
			}
			if( u == TARGET ){
				// if we pop the target off of the priority queue, then we know it is the minimum distance!
				distance = next.length;
				return true; 
			}
			// push all of the neighbor's of the current node into the frontier
			map_pair = adjacency_list.find(next.neighbor_node);
			if( map_pair != adjacency_list.end() ){
				for (vector<edge>::const_iterator v=(map_pair->second).begin(); v!=(map_pair->second).end(); ++v){
					// don't push node onto frontier if it's marked
					if( marked.count(v->neighbor_node)==0 ){
						frontier.push(edge(v->street_name,v->length+next.length,v->neighbor_node));
					}
				}
			}
		}
	}
	return false;
}

bool street_map::route(int su, int sv, float spos, int tu, int tv, float tpos, vector<pair<string, float>> &steps) const {
	// steps is a vector of <pair<streetname,distance>>
	steps.clear();

	// use a modified dijkstra's algorithm
	bool pathFound = false; 	// true if a path from SOURCE to TARGET is found
	float s1, t1; 			// length of edge (su,sv), (tu,tv)
	string s_street, t_street;	// street name that (su,sv) and (tu,tv) are on

	// calculate lengths of segments that s and v are on
	// calculate length of (su,sv)
	unordered_map<int,vector<edge>>::const_iterator map_pair = adjacency_list.find(su);
	vector<edge>::const_iterator it;
	for (it=(map_pair->second).begin(); it!=(map_pair->second).end(); ++it){
		if( it->neighbor_node==sv ) break;
	}
	if( it==(map_pair->second).end() ) return false;//b/c the node pair does not exist in adjacency list
	s1 = it->length;
	s_street = it->street_name;
	// calculate length of (tu,tv)
	map_pair = adjacency_list.find(tu);
	for (it=(map_pair->second).begin(); it!=(map_pair->second).end(); ++it){
		if( it->neighbor_node==tv ) break;
	}
	if( it==(map_pair->second).end() ) return false;//b/c the node pair does not exist in adjacency list
	t1 = it->length;
	t_street = it->street_name;

	// define arbitrary nodes
	int SOURCE=-2, TARGET=-1;

	// handle case where SOURCE and TARGET lie on same segment
	// if true, then we already know the path from SOURCE to TARGET! So simply push_back() on steps and then return true
	if( su==tu && sv==tv ){
		steps.push_back(pair<string,float>(s_street, abs(spos-tpos)));
		return true;
	}
	if( su==tv && sv==tu ){
		steps.push_back(pair<string,float>(s_street, abs(s1-(spos+tpos))));
		return true;
	}

	// begin modified dijkstra's
	priority_queue<edge,vector<edge>,less<edge> > frontier; // frontier uses std priority queue to store edges
	unordered_map<int,int> marked; // marked: <destination,previos_hop>
	// Push source node with distance of zero, along with two adjacent nodes
	frontier.push(edge("",0,SOURCE));
	frontier.push(edge("",spos,su,SOURCE));
	frontier.push(edge("",s1-spos,sv,SOURCE));
	while(!frontier.empty()){
		edge next = frontier.top();
		int u = next.neighbor_node;
		frontier.pop();
		// make sure that the node popped off frontier is not already marked
		if( marked.count(u)==0 ){
			marked.insert(pair<int,int>(u,next.prev_hop));
			// take care of special cases where TARGET is a neighbor (b/c TARGET is not actually a node in adjacency_list)
			if( u == tu ){
				frontier.push(edge(t_street,next.length+tpos,TARGET,u));
			}
			if( u == tv ){
				frontier.push(edge(t_street,abs(next.length+t1-tpos),TARGET,u));
			}
			if( u == TARGET ){
				// if we pop the target off of the priority queue, then we know it is the minimum distance!
				pathFound = true; 
				break;
			}
			// push all of the neighbor's of the current node into the frontier
			map_pair = adjacency_list.find(next.neighbor_node);
			if( map_pair != adjacency_list.end() ){
				for (vector<edge>::const_iterator v=(map_pair->second).begin(); v!=(map_pair->second).end(); ++v){
					// don't push node onto frontier if it's marked
					if( marked.count(v->neighbor_node)==0 ){
						frontier.push(edge(v->street_name,v->length+next.length,v->neighbor_node,u));
					}
				}
			}
		}
	}
	// if a path was not found, then return false
	if( !pathFound ) return pathFound;//return false

	// else, build the path vector and then return true (ie. pathFound)
	// begin with the pair in marked that contains the TARGET as the first element, then follow the prev_hop's of each pair in marked until the first element is the SOURCE
	unordered_map<int,int>::const_iterator bp = marked.find(TARGET);
	// begin with first case, where segment is (tu,tv) (b/c TARGET to tu/tv are not neighbors in the adjacency_list)
	if(bp->second==tu){
		steps.push_back(pair<string,float>(t_street,tpos));
		bp=marked.find(tu);
	}
	if(bp->second==tv){
		steps.push_back(pair<string,float>(t_street,t1-tpos));
		bp=marked.find(tv);
	}
	// now continue to backtrack through marked (using prev_hop) until we get to the SOURCE
	for( ; bp->first!=SOURCE; bp=marked.find(bp->second) ){
		// handle special case of (SOURCE,su) and (SOURCE,sv) b/c SOURCE is not stored in adjacency_list
		if( bp->second == SOURCE ){
			if( bp->first==su ){
				if( (steps.back()).first == s_street ){
					(steps.back()).second += spos;
				}else{
					steps.push_back(pair<string,float>(s_street,spos));
				}
			}
			if( bp->first==sv ){
				if( (steps.back()).first == s_street ){
					(steps.back()).second += (s1-spos);
				}else{
					steps.push_back(pair<string,float>(s_street,s1-spos));
				}
			}
		}else{
		// normal case (where SOURCE is not part of segment)
			map_pair = adjacency_list.find(bp->first);
			for( it = (map_pair->second).begin(); it!=(map_pair->second).end(); ++it ){
				if(it->neighbor_node==bp->second){ //then we've found the pair from marked
					// determine if it's still the same street; if so, then just increment distance value; else, push back new street on steps
					if( (steps.back()).first == it->street_name ){
						(steps.back()).second += it->length;
					}else{
						steps.push_back(pair<string,float>(it->street_name,it->length));
					}
					break;
				}
			}
		}
	}
	// reverse because we built path from TARGET to SOURCE. We actually want path from SOURCE to TARGET
	reverse(steps.begin(),steps.end());

	return pathFound;//true
}
