#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h> // for isatty

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

int main (int argc, char *argv[]) {
  while (!cin.eof()) {
    if (interactive) cout << "> ";

    string s;
    getline(cin, s);
    linenum++;

    double x;
    if (string_to_number(s, x) == 0) {
    } else if (s == "") {
      // do nothing
    } else {
      error("couldn't understand input");
    }
  }
}
