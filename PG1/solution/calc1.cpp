#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h> // for isatty

#include <stack>

using namespace std;

const bool interactive = isatty(0);
int linenum = 0;

void error (const char *s) {
  if (interactive) {
    cout << "error: " << s << endl;
  } else {
    cerr << "error: " << s << " on line " << linenum << endl;
  }
}

template <typename Number>
int string_to_number (const string &s, Number &x) {
  stringstream ss(s);
  ss >> x;
  if (ss.eof() && !ss.fail()) 
    return 0;
  else
    return -1;
}

template <typename T>
int pop(stack<T>& stk, T& x) {
  if (stk.empty()) {
    error("stack underflow");
    return -1;
  } else {
    x = stk.top();
    stk.pop();
    return 0;
  }
}

int main (int argc, char *argv[]) {
  stack<double> stk;

  while (!cin.eof()) {
    if (!stk.empty()) cout << stk.top() << endl;
    if (interactive) cout << "> ";

    string s;
    getline(cin, s);
    linenum++;

    double x, y;
    if (string_to_number(s, x) == 0) {
      stk.push(x);
    } else if (s == "+") {
      if (pop(stk, y) == 0 && pop(stk, x) == 0)
	stk.push(x + y);
    } else if (s == "-") {
      if (pop(stk, y) == 0 && pop(stk, x) == 0)
	stk.push(x - y);
    } else if (s == "*") {
      if (pop(stk, y) == 0 && pop(stk, x) == 0)
	stk.push(x * y);
    } else if (s == "/") {
      if (pop(stk, y) == 0 && pop(stk, x) == 0)
	stk.push(x / y);
    } else if (s == "drop") {
      pop(stk, x);
    } else if (s == "swap") {
      if (pop(stk, y) == 0 && pop(stk, x) == 0) {
	stk.push(y);
	stk.push(x);
      }
    } else if (s == "") {
      // do nothing
    } else {
      error("couldn't understand input");
    }
  }
}
