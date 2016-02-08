#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h> // for isatty
#include <stack>

using namespace std;

//BEGIN NODE DECLARATION
template <typename Number>
class Node {
	public:
		Node(Number);
		Number get_val();
		Node* get_next();
		void set_next(Node*);
	private:
		Number val;
		Node* next;
};

template <typename Number>
Node<Number>::Node(Number my_val) {
	val = my_val;
	next = NULL;
}

template <typename Number>
Number Node<Number>::get_val() {
	return val;
}

template <typename Number>
Node<Number>* Node<Number>::get_next() {
	return next;
}

template <typename Number>
void Node<Number>::set_next(Node* new_next) {
	next = new_next;
}

//BEGIN STACK DECLARATION
template <typename Number>
class myStack {
	public:
		myStack();
		Number top();
		void pop();
		void push(Number);
		bool empty();
		int size();
	private:
		Node<Number>* top_ptr;
		int mySize;
};

template <typename Number>
myStack<Number>::myStack() {
	mySize = 0;
	top_ptr = NULL;
}

template <typename Number>
Number myStack<Number>::top() {
	if(top_ptr != NULL)
		return top_ptr->get_val();
}

template <typename Number>
void myStack<Number>::pop() {
	if(top_ptr != NULL){
		top_ptr = top_ptr->get_next();

		mySize--;
	}
}

template <typename Number>
void myStack<Number>::push(Number new_val) {
	Node<Number>* new_node = new Node<Number>(new_val);
	if(top_ptr != NULL){
		new_node->set_next(top_ptr);
	}
	top_ptr = new_node;

	mySize++;
}

template <typename Number>
bool myStack<Number>::empty() {
	bool is_empty = false;
	if( top_ptr == NULL ) is_empty = true;
	return is_empty;
}

template <typename Number>
int myStack<Number>::size() {
	return mySize;
}

//BEGIN FUNCTION DECLARATIONS
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
	myStack<double> rpn1;
	bool showTop, stackChanged;
	myStack<myStack<double> > undo_stack;

	while (!cin.eof()) {
		showTop = true;
		stackChanged = true;
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
				stackChanged = false;
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
				stackChanged = false;
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
				stackChanged = false;
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
				stackChanged = false;
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
			stackChanged = false;
		}
		if( str2lower(s) != "undo" ){
		// only add to the undo stack if "undo" was not called
			if(!undo_stack.empty() ) {
			// this check is necessary to prevent top() on an empty undo_stack 
				if( stackChanged ){
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
