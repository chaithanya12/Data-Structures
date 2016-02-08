#include <iostream>

template <typename T>
class shared_stack {
public:
  typedef T element_type;
 
private:
  struct node {
    element_type value;
    node* next;
    node(const T& value, node* next) : value(value), next(next) { }
  };
  node* head;

public:
  shared_stack() : head(nullptr) { }
  shared_stack(const shared_stack<T>& other) : head(other.head) { }

  T top() const { return head->value; }
  bool empty() const { return head == nullptr; }
  void pop() { head = head->next; }
  void push(const T& value) { head = new node(value, head); }
};
