/* quadtree.hpp
 * Edited by Patrick Drumm
 *
 * FORMATTING: Use ":set ts=2" to align tabs with original spacing of provided skeleton
 *
 * class that implements a search tree where each node has up to four children
 * - allows new locations to be added while still keeping the order of the tree
 * - contains a search that will find only the locations within a given radius (of a given location)
 */

#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <utility>
#include <string>
#include <vector>

#include "distance.hpp"
using namespace std;

class quadtree {
  // A point quadtree for locations on Earth.

  // Notes: 
  // - Assume a spherical Earth. But assume that if you keep traveling
  //   west, you will hit longitude -180 and stop, and if you keep
  //   traveling east, you will hit longitude +180 and stop; you don't
  //   have to worry about wrapping around like Magellan.
  // - Locations are measured in degrees latitude and longitude, but
  //   distances are measured in miles.

private:
	point bounding_sw=point(-180,-90), bounding_ne=point(180,90);
	typedef struct tree_node{
		string name;
		point location;
		struct tree_node *NE, *ES, *SW, *WN;
	} Node;
	Node *root;

public:
  // See bintree.hpp for skeleton definitions of the default
  // constructor, copy constructor, destructor, and copy assignment
  // operator (and swap).

	// Default constructor
	quadtree() {
		root = NULL;
	}

	// Copy constructor
	quadtree(const quadtree &t) {
		root = NULL;
		if( t.root!=NULL )
			copy_nodes(t.root);
	}
	// Copy constructor helper function - recursively builds copy constructed tree
	void copy_nodes(Node *t_cursor){
		insert( (*t_cursor).name, (*t_cursor).location );
		if( (*t_cursor).NE!=NULL )
			copy_nodes( (*t_cursor).NE );
		if( (*t_cursor).ES!=NULL )
			copy_nodes( (*t_cursor).ES );
		if( (*t_cursor).SW!=NULL )
			copy_nodes( (*t_cursor).SW );
		if( (*t_cursor).WN!=NULL )
			copy_nodes( (*t_cursor).WN );
	}

	// Destructor
	~quadtree() {
		if(root!=NULL)
			delete_node(root);
	}
	// Deconstructor helper function - recursively deletes nodes from leaf to root
	void delete_node(Node *rm_node){
		if( (*rm_node).NE!=NULL )
			delete_node( (*rm_node).NE );
		if( (*rm_node).ES!=NULL )
			delete_node( (*rm_node).ES );
		if( (*rm_node).SW!=NULL )
			delete_node( (*rm_node).SW );
		if( (*rm_node).WN!=NULL )
			delete_node( (*rm_node).WN );
		delete rm_node;
	}

	// Copy assignment
	quadtree &operator= (quadtree other) {
		swap(*this, other);
		return *this;
	}
	friend void swap(quadtree &t1, quadtree &t2) {
		using std::swap;
		swap(t1.root, t2.root);
	}

  void insert(const std::string& name, point p) {
    // Insert an element with name `name` and location `p`.
		Node *new_node = new Node;
		// Add input information to new node
		(*new_node).name = name;
		(*new_node).location = p;
		(*new_node).NE = NULL;
		(*new_node).ES = NULL;
		(*new_node).SW = NULL;
		(*new_node).WN = NULL;
		// Traverse tree, then insert node
		if( root==NULL ){
			root = new_node;
		}else{
			Node *cursor = root, *prev_cur;
			while( cursor!=NULL ){
				prev_cur = cursor;
				// Note that if a coordinate happens to lie on the same x or y axis as the location being added, then the location being added will be put into the sector where the matching axis matches the first letter of the Node pointer
				// eg. a node that is directly south will be in the SW subtree. a node that is directly west will be in the WN subtree.
				if( p.lon>=(*cursor).location.lon && p.lat>(*cursor).location.lat )
					cursor = (*cursor).NE;
				else if( p.lat<=(*cursor).location.lat && p.lon>(*cursor).location.lon)
					cursor = (*cursor).ES;
				else if( p.lat<(*cursor).location.lat )
					cursor = (*cursor).SW;
				else
					cursor = (*cursor).WN;
			}
			// Insert new node
			if( p.lon>=(*prev_cur).location.lon && p.lat>(*prev_cur).location.lat )
				(*prev_cur).NE = new_node;
			else if( p.lat<=(*prev_cur).location.lat && p.lon>(*prev_cur).location.lon)
				(*prev_cur).ES = new_node;
			else if( p.lat<(*prev_cur).location.lat )
				(*prev_cur).SW = new_node;
			else
				(*prev_cur).WN = new_node;
		}
  }

  void within_radius(point p, double r, std::vector<std::string> &result) const{
    // Search for elements within the circle with center `p` (in
    // degrees latitude and longitude) and radius `r` (in miles).
    // Clears `result` and puts the elements in `result`.
    // Postcondition: `result` contains all (and only) the elements of
    // the tree, in any order, that lie within the circle defined by
    // `p` and `r`.
		result.clear();
		search(p,r,result,root,bounding_sw,bounding_ne);
  }
	// within_radius helper function - recursively searches subtrees of the node cursor points to
	void search(point p, double r, vector<string> &result, Node *cursor, point sw, point ne) const{
		/* distance(point 1, point 2)
		 * 		- computes the distance (in miles) between two points (just as in PG2).
		 * distance_to_box(center of circle, sw box corner, ne box corner)
		 * 		- computes the minimum distance (in miles) from a point to a box(defined by its southwest and northeast corners).
		 */
		if(cursor==NULL) return;
		// if brange overlaps with srange
		if( distance_to_box(p,sw,ne) <= r ){
			if( distance(p,(*cursor).location) <= r ){
				result.push_back( (*cursor).name );
			}
			search(p,r,result,(*cursor).NE,(*cursor).location,ne);
			search(p,r,result,(*cursor).ES,point(sw.lat,(*cursor).location.lon),point((*cursor).location.lat,ne.lon));
			search(p,r,result,(*cursor).SW,sw,(*cursor).location);
			search(p,r,result,(*cursor).WN,point((*cursor).location.lat,sw.lon),point(ne.lat,(*cursor).location.lon));
		}
	}
};

#endif
