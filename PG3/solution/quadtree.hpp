#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <iostream>

#include <string>
#include <vector>
#include <cmath>
#include <cassert>
#include "distance.hpp"

typedef enum { SW=0, SE=1, NW=2, NE=3 } quadrant;

class quadtree {
  struct node {
    node *child[4];
    std::string name;
    point p;

    node(const std::string& name, point p) : name(name), p(p) {
      for (int q=0; q<4; q++) child[q] = nullptr;
    }
    node (const node &other) : name(other.name), p(other.p) {
      for (int q=0; q<4; q++)  {
	if (other.child[q])
	  child[q] = new node(*other.child[q]);
	else
	  child[q] = nullptr;
      }
    }
    ~node() {
      for (int q=0; q<4; q++)
	if (child[q])
	  delete child[q];
    }

    friend void swap(node &n1, node &n2) {
      std::swap(n1.name, n2.name);
      std::swap(n1.p, n2.p);
      for (int q=0; q<4; q++)
	std::swap(n1.child[q], n2.child[q]);
    }
    node &operator= (node other) { swap(*this, other); return *this; }

  };

  node *root;

public:
  quadtree() : root(nullptr) { }
  quadtree(const quadtree &other) {
    root = new node(*other.root);
  }
  ~quadtree() { delete root; }

  friend void swap(quadtree &t1, quadtree &t2) {
    std::swap(t1.root, t2.root);
  }
  quadtree &operator= (quadtree other) { swap(*this, other); return *this; }

  void insert(const std::string& name, point p) {
    node *cur = root;
    node *prev = nullptr;
    quadrant q;
    while (cur != nullptr) {
      q = static_cast<quadrant>((p.lat > cur->p.lat) * 2 + (p.lon > cur->p.lon));
      prev = cur;
      cur = cur->child[q];
    }
    if (prev == nullptr) {
      root = new node(name, p);
    } else {
      prev->child[q] = new node(name, p);
    }
  }

  void within_radius(point p, double radius, std::vector<std::string> &result) const {
    point sw(-90, -180), ne(90, 180);
    result.clear();
    within_radius_visit(p, radius, result, root, sw, ne);
  }

private:

  void within_radius_visit(point p, double r, std::vector<std::string> &result, 
			    const node *cur, const point &sw, const point &ne) const {
    if (!cur)
      return;

    if (distance_to_box(p, sw, ne) > r)
      return;
    if (distance(p, cur->p) <= r)
      result.push_back(cur->name);
    within_radius_visit(p, r, result, cur->child[SW], sw, cur->p);
    within_radius_visit(p, r, result, cur->child[SE], 
			point(sw.lat, cur->p.lon), 
			point(cur->p.lat, ne.lon));
    within_radius_visit(p, r, result, cur->child[NW], 
			point(cur->p.lat, sw.lon),
			point(ne.lat, cur->p.lon)); 
    within_radius_visit(p, r, result, cur->child[NE], cur->p, ne);
  }

};

#endif
