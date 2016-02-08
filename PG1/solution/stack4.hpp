#include <memory>
#include <iostream>

template <typename T>
class shared_stack {
public:
  typedef T element_type;
 
private:
  struct node {
    element_type value;
    std::shared_ptr<node> next;
    node(const T& value, const std::shared_ptr<node>& next) : value(value), next(next) { }
  };
  std::shared_ptr<node> head;

public:
  shared_stack() : head(nullptr) { }
  shared_stack(const shared_stack<T>& other) : head(other.head) { }
  ~shared_stack() { 
    // To avoid too-deep recursion, we iteratively pop as long as head
    // is the only owner of the node it points to
    while (!empty() && head.unique()) pop();
  }

  T top() const { return head->value; }
  bool empty() const { return head == nullptr; }
  void pop() { head = head->next; }
  void push(const T& value) { head = std::make_shared<node>(value, head); }
};
