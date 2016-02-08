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

double calcResult (const string s, double num1, double num2) {
	double result;
	if (s=="+") {
		result = num1 + num2;
	} else if (s=="-") {
		result = num1 - num2;
	} else if (s=="*") {
		result = num1 * num2;
	} else if (s=="/") {
		result = num1 / num2;
	}
	return result;
}

string str2lower(string s) {
	for( int i=0; i<s.size(); i++ ) {
		s[i] = tolower(s[i]);
	}
	return s;
}

int main (int argc, char *argv[]) {
	stack<double> rpn1;
	bool showTop;
	stack<stack<double> > undo_stack;

	while (!cin.eof()) {
		showTop = true;
		if (interactive) cout << "> ";

		string s;
		getline(cin, s);
		linenum++;

		double x;
		if (string_to_number(s, x) == 0) {
			rpn1.push(x);
		} else if (s == "") {
			// do nothing
		} else if (s=="+" || s=="-" || s=="*" || s=="/" ) {
		// arithmetic operations
			if(rpn1.size() < 2) {
				cout << "error: stack underflow on line " << linenum << endl;
				showTop = false;
			} else {
				double num1, num2;
				// operation will follow
				// 	num1 's' num2
				// 	thus, num2 will be popped before num1
				num2 = rpn1.top();
				rpn1.pop();
				num1 = rpn1.top();
				rpn1.pop();

				rpn1.push( calcResult(s,num1,num2) );
			}
		} else if (str2lower(s) == "drop") {
		// drop
			if(rpn1.size() < 1) {
				cout << "error: stack underflow on line " << linenum << endl;
				showTop = false;
			} else {
				rpn1.pop();
				if(rpn1.size() < 1) {
					showTop = false;
				}
			}
		} else if (str2lower(s) == "swap") {
		// swap
			if(rpn1.size() < 2) {
				cout << "error: stack underflow on line " << linenum << endl;
				showTop = false;
			} else {
				double temp1, temp2;
				temp1 = rpn1.top();
				rpn1.pop();
				temp2 = rpn1.top();
				rpn1.pop();
				rpn1.push(temp1);
				rpn1.push(temp2);
			}
		} else if (str2lower(s) == "undo") {
		// undo
			if(undo_stack.empty()){
				error("cannot undo");
				showTop = false;
			} else {
				undo_stack.pop();
				if(undo_stack.empty()){
					rpn1.pop();	// pop the element in rpn1 b/c the only way that undo could be empty now is if the last move was adding an element
					showTop = false;
				} else {
					rpn1 = undo_stack.top();
					if(rpn1.size() < 1 ) { showTop = false; }
				}
			}
		} else {
			error("couldn't understand input");
			showTop = false;
		}
		if( str2lower(s) != "undo" ){
		// only add to the undo stack if "undo" was not called
			if(!undo_stack.empty() ) {
			// this check is necessary to prevent top() on an empty undo_stack 
				if( undo_stack.top() != rpn1 ){
					undo_stack.push(rpn1);
				}
			} else {
				if( !rpn1.empty() ) {
				// this check is necessary to prevent push on undo stack when rpn is empty (b/c an error was outputted)
					undo_stack.push(rpn1);
				}
			}
		}
		// if there has not been an error outputted and the stack isnt empty and cin!=eof, show the top element
		if (showTop && !cin.eof() ) {
			cout << rpn1.top() << endl;
		}
	}
}
