/* bintree.hpp
 * Edited by Patrick Drumm
 *
 * FORMATTING: Use ":set ts=2" to align tabs with original spacing of provided skeleton
 *
 * class implementing a binary search tree
 * - allows for new nodes to be inserted properly
 * - provides a search function that will return all locations found within a given radius (from a given location)
 */

#ifndef BINTREE_HPP
#define BINTREE_HPP

#include <utility>
#include <string>
#include <vector>
using namespace std;

class bintree {
  // A binary search tree for locations in Lineland.

  // Notes: 
  // - Assume a flat, one-dimensional world with locations from -180 to 180.
  // - All locations and distances are measured in the same units (degrees).

private:
	double bounding_min=-180, bounding_max=180;
	typedef struct tree_node{
		string name;
		double location;
		struct tree_node *left, *right;
	} Node;
	Node *root;

public:
  // Default constructor
  bintree() {
		root= NULL;
  }

  // Copy constructor
  bintree(const bintree &t) {
    // Copy all data members here. It should normally be a deep copy,
    // i.e., the things that pointers point to should be copied as
    // well.
		root = NULL;
		if(t.root!=NULL)
			copy_nodes(t.root);
  }
	// Copy constructor helper function - recursively builds copy constructed tree
	void copy_nodes(Node *t_cursor){
		insert( (*t_cursor).name, (*t_cursor).location );
		if( (*t_cursor).left!=NULL ){
			copy_nodes( (*t_cursor).left );
		}
		if( (*t_cursor).right!=NULL ){
			copy_nodes( (*t_cursor).right );
		}
	}

  // Destructor
  ~bintree() {
		if(root!=NULL)
			delete_node(root);
  }
	// Deconstructor helper function - recursively deletes nodes from leaf to root
	void delete_node(Node *rm_node) {
		if( (*rm_node).left!=NULL )
			delete_node((*rm_node).left);
		if( (*rm_node).right!=NULL )
			delete_node((*rm_node).right);
		delete rm_node;
	}

  // Copy assignment is implemented using the copy-swap idiom
  // https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-and-swap
  bintree &operator= (bintree other) {
    // You don't need to modify this function.
    swap(*this, other);
    return *this;
  }
  friend void swap(bintree &t1, bintree &t2) {
    using std::swap;
    // Swap all data members here, e.g.,
    // swap(t1.foo, t2.foo);
    // Pointers should be swapped -- but not the things they point to.
    //
    // To swap trees, simply exchange root pointers
    swap(t1.root, t2.root);
  }

  void insert(const std::string& name, double p) {
    // Insert an element with name `name` and location `p`.
    Node *new_node = new Node;
		// Add input information to new node
		(*new_node).name = name;
		(*new_node).location = p;
		(*new_node).left = NULL;
		(*new_node).right = NULL;
		// Traverse tree, then insert node
		if (root==NULL){
			root = new_node;
		}else{
			Node *cursor = root, *prev_cur;
			while ( cursor!=NULL ){
				prev_cur = cursor;
				if( p > (*cursor).location )
					cursor = (*cursor).right;
				else
					cursor = (*cursor).left;
			}
			// Insert new node
			if( p > (*prev_cur).location )
				(*prev_cur).right = new_node;
			else
				(*prev_cur).left = new_node;
		}
  }

  void within_radius(double p, double r, std::vector<std::string> &result) const {
    // Search for elements within the range `p` plus or minus `r`.
    // Clears `result` and puts the elements in `result`.
    // Postcondition: `result` contains all (and only) elements of the
    // tree, in any order, that lie within the range `p` plus or minus
    // `r`.
    result.clear();
		search(p,r,result,root,bounding_min,bounding_max);
  }
	// within_radius helper function - recursively searches subtrees of the node cursor points to
	void search(double p, double r, vector<string> &result, Node *cursor, double bmin, double bmax) const{
		if(cursor==NULL) return;
		// if brange overlaps with srange
		if( bmin<=(p+r) || bmax>=(p-r) ){
			if( (*cursor).location>=(p-r) && (*cursor).location<=(p+r) ){
				result.push_back( (*cursor).name );
			}
			search(p,r,result,(*cursor).left,bmin,(*cursor).location);
			search(p,r,result,(*cursor).right,(*cursor).location,bmax);
		}
	}
};

#endif
