#ifndef BINTREE_HPP
#define BINTREE_HPP

#include <iostream>

#include <string>
#include <vector>
#include <cmath>
#include <cassert>

class bintree {
  struct node {
    node *child[2];
    std::string name;
    double p;

    node(const std::string& name, double p) : name(name), p(p) { child[0] = child[1] = nullptr; }
    node(const node &other) : name(other.name), p(other.p) {
      for (int i=0; i<2; i++) {
	if (other.child[i])
	  child[i] = new node(*other.child[i]);
	else
	  child[i] = nullptr;
      }
    }
    ~node() { 
      for (int i=0; i<2; i++)
	if (child[i])
	  delete child[i];
    }

    friend void swap(node &n1, node &n2) {
      std::swap(n1.name, n2.name);
      std::swap(n1.p, n2.p);
      std::swap(n1.child[0], n2.child[0]);
      std::swap(n1.child[1], n2.child[1]);
    }
    node &operator= (node other) { swap (*this, other); return *this; }

  };

  node *root;

public:
  bintree() : root(nullptr) { }
  bintree(const bintree &other) {
    root = new node(*other.root);
  }
  ~bintree() { delete root; }

  friend void swap(bintree &t1, bintree &t2) {
    std::swap(t1.root, t2.root);
  }
  bintree &operator= (bintree other) { swap(*this, other); return *this; }

  void insert(const std::string& name, double p) {
    node *cur = root;
    node *prev = nullptr;
    int d;
    while (cur != nullptr) {
      d = p < cur->p ? 0 : 1;
      prev = cur;
      cur = cur->child[d];
    }
    if (prev == nullptr) {
      root = new node(name, p);
    } else {
      prev->child[d] = new node(name, p);
    }
  }

  void within_radius(double p, double radius, std::vector<std::string> &result) const {
    result.clear();
    within_radius_visit(p, radius, result, root, -180, 180);
  }

private:

  void within_radius_visit(double p, double r, std::vector<std::string> &result, 
			   const node *cur, double w, double e) const {
    if (!cur)
      return;

    if (p+r < w || p-r > e)
      return;
    if (fabs(p - cur->p) <= r)
      result.push_back(cur->name);
    within_radius_visit(p, r, result, cur->child[0], w, cur->p);
    within_radius_visit(p, r, result, cur->child[1], cur->p, e);
  }

};

#endif
