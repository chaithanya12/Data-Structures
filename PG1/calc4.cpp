/*
 * 	calc4.cpp
 *	Author: Patrick Drumm
 *
 *	The best way to explain calc4.cpp is to begin by describing how it defers from calc3.cpp. This program rids the need for two separate stacks by building a unique and specialized stack class implementation for this program. In order to do so, a few additional data members and functions were implemented.
 *		- The main way that I was able to make swaps and drops possible when using a single stack was by introducing variables in the Node class to keep track of each. If a swap ever occured, the program would swap the two top variables, and then increment both of their 'swap counts'. This way, a future 'undo' would be able to tell if and how many times a swap was called.
 *		- In order to make drops possible, I utilized a 'isDrop' boolean. My program actually adds a new node to the stack when a "drop" is called. This new node will have isDrop=true, so that future 'undo' commands would know when to 'add' the dropped node back onto the stack.
 *
 *	Furthermore, the progression of my coding and how I arrived at my final program led me to create two major additional functions for the myStack class. These two commands are:
 *		void undo_op_push(string,Number);
 *		void undo_op_pop();
 *			- Essentially, undo_op_POP is called whenever the user enters "undo", and undo_op_PUSH is called for any other (valid) commands. I thought of it as a "push" and "pop" because one is adding onto the stack(because even "drop" adds to the stack in my program!) while the other is backtracking and removing.
 *
 */

#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h> // for isatty
#include <stack>
#include <memory>

using namespace std;
string str2lower(string);

//BEGIN NODE DECLARATION
template <typename Number>
class Node {
	public:
		Node(Number,bool=false,shared_ptr<Node<Number> > =NULL);	// Constructor
		Number get_val();						// return val
		shared_ptr<Node<Number> > get_next();				// return next node in the 'calculator' stack. Eg. if user enters: 1 2 3 + then top_ptr would point to 5 and top_ptr->get_next() would point to 1
		shared_ptr<Node<Number> > get_adj_node();			// return next node in 'undo' stack. Eg. if user enters: 1 2 3 + then top_ptr would point to 5 and top_ptr->get_adj_node() would point to 3
		void set_next(shared_ptr<Node<Number> >);			// set where the node points to
		int get_swaps();						// return swaps - number of time the node has been swapped
		bool get_isDrop();						// return isDrop - true if the node represents a previous node that was dropped
		void get_swapped();						// swaps++
		void unswap();							// swaps--
	private:
		Number val;
		shared_ptr<Node<Number> > next;
		int swaps;
		bool isDrop;		
		shared_ptr<Node<Number> > adj_node;
};

template <typename Number>
Node<Number>::Node(Number my_val, bool my_isDrop, shared_ptr<Node<Number> > my_adj_node) {
	val = my_val;
	next = NULL;
	swaps = 0;
	isDrop = my_isDrop;
	adj_node = my_adj_node;
}

template <typename Number>
Number Node<Number>::get_val() {
	return val;
}

template <typename Number>
shared_ptr<Node<Number> > Node<Number>::get_next() {
	return next;
}

template <typename Number>
shared_ptr<Node<Number> > Node<Number>::get_adj_node() {
	return adj_node;
}

template <typename Number>
void Node<Number>::set_next(shared_ptr<Node<Number> > new_next) {
	next = new_next;
}

template <typename Number>
int Node<Number>::get_swaps() {
	return swaps;
}

template <typename Number>
bool Node<Number>::get_isDrop() {
	return isDrop;
}

template <typename Number>
void Node<Number>::get_swapped() {
	++swaps;
}

template <typename Number>
void Node<Number>::unswap() {
	--swaps;
}

//BEGIN STACK DECLARATION
template <typename Number>
class myStack {
	public:
		myStack();
		~myStack();
		Number top();
		void pop();
		void push(Number);
		bool empty();
		int size();
		void undo_op_push(string,Number);	// "undo" was not called. add onto the stack
		void undo_op_pop();			// "undo" was called. remove/backtrack from the stack
		void find_2_nums(Number*,Number*);
	private:
		shared_ptr<Node<Number> > top_ptr;
		int mySize;
};

template <typename Number>
myStack<Number>::myStack() {
	mySize = 0;
	top_ptr = NULL;
}

template <typename Number>
myStack<Number>::~myStack() {
	while(!empty() && top_ptr.unique()) pop();
}

template <typename Number>
Number myStack<Number>::top() {
	if(top_ptr != NULL){
		shared_ptr<Node<double> > temp = top_ptr;
		while(temp->get_isDrop()){
			temp = temp->get_next();
			temp = temp->get_next();
		}
		return temp->get_val();
	}
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
	auto new_node = make_shared<Node<Number> >(new_val);
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

template <typename Number>
void myStack<Number>::undo_op_push(string undo_op, Number new_val){
	int op_type = 0;
	// op_type can be thought of as the number of nodes that must be counted backwards (in the path of pointers) for the new node to point at
		// zero op:	swap 		- no new node is added
		// one op:	push, drop 	- enter a new number, or a isDrop=true node both point to previous node in path
		// three op:	+ - * /		- arithmetic operations must jump/point over the two nodes used to calculate the result
	if(str2lower(undo_op)=="drop"){
		op_type = 1;
		shared_ptr<Node<Number> > temp_ptr = top_ptr;
		while(temp_ptr->get_isDrop()){
			temp_ptr = temp_ptr->get_next();
			temp_ptr = temp_ptr->get_next();
		}
		new_val = temp_ptr->get_val();
	}else if(undo_op=="+" || undo_op=="-" || undo_op=="*" || undo_op=="/"){
		op_type = 3;
	}else if(str2lower(undo_op)=="swap"){
		op_type = 0;
		top_ptr->get_swapped();
		top_ptr->get_next()->get_swapped();

		shared_ptr<Node<Number> > move_head = top_ptr, prev_head;
		while(move_head->get_isDrop() ){
			move_head = move_head->get_next();
			prev_head = move_head;
			move_head = move_head->get_next();
		}
		shared_ptr<Node<Number> > temp_ptr = move_head->get_next();
		move_head->set_next(move_head->get_next()->get_next());
		temp_ptr->set_next(move_head);
		if(move_head == top_ptr) top_ptr = temp_ptr;
		else prev_head->set_next(temp_ptr);
	}else{//push
		op_type = 1;
	}
	//This loop helps to treat the isDrop=true nodes as "nonexistant" by skipping over them
	int i=0;
	bool first_loop = true;
	shared_ptr<Node<Number> > cur_ptr = top_ptr;
	while(i!=op_type){
		if(first_loop) first_loop=false;
		else cur_ptr = cur_ptr->get_next();

		if(cur_ptr==NULL) break;

		if(cur_ptr->get_isDrop()){
			cur_ptr = cur_ptr->get_next();
		}else{
			i++;
		}
	}
	//now the pointer is pointing where it should be for the new Node in the undo list
	if(op_type!=0){
		bool isDrop = (str2lower(undo_op)=="drop")?true:false;
		shared_ptr<Node<Number> > adj_node = (op_type==3 || op_type==1)?top_ptr:NULL;
		auto new_node = make_shared<Node<Number> >(new_val,isDrop,adj_node);
		if(top_ptr != NULL){
			new_node->set_next(cur_ptr);
		}
		top_ptr = new_node;
		if(isDrop) mySize-=2;
		if(op_type==3) mySize-=2;
		mySize++;		
	}
}

template <typename Number>
void myStack<Number>::undo_op_pop() {
	if(top_ptr->get_isDrop() && top_ptr->get_swaps()<1){
	//Undo a drop
		top_ptr = top_ptr->get_adj_node();
		++mySize;
	}else if(top_ptr->get_swaps() > 0) {
	//undo a swap
		top_ptr->unswap();
		top_ptr->get_next()->unswap();

		shared_ptr<Node<Number> > move_head = top_ptr, prev_head;
		while(move_head->get_isDrop() ){
			move_head = move_head->get_next();
			prev_head = move_head;
			move_head = move_head->get_next();
		}
		shared_ptr<Node<Number> > temp_ptr = move_head->get_next();
		move_head->set_next(move_head->get_next()->get_next());
		temp_ptr->set_next(move_head);
		if(move_head == top_ptr) top_ptr = temp_ptr;
		else prev_head->set_next(temp_ptr);

	}else{
	//undo a arithmetic operation or simple number entry
		shared_ptr<Node<Number> > target_node = top_ptr->get_next();
		Number op_val_1, op_val_2;
		top_ptr = top_ptr->get_adj_node();
		--mySize;

		//i will get to 2 if the operation being undo'ed is an arithmetic operation
		int i=0;
		for(shared_ptr<Node<Number> > cur_node=top_ptr; cur_node != target_node; cur_node=cur_node->get_next()){
			if(cur_node->get_isDrop()) cur_node=cur_node->get_next();
			else ++i;
			if(i==1) op_val_1=cur_node->get_val();
			if(i==2) op_val_2=cur_node->get_val();
		}
		if(i>0){
			mySize+=2;
		}
	}
}

template <typename Number>
void myStack<Number>::find_2_nums(Number* op_1,Number* op_2){
	int i=0;
	for(shared_ptr<Node<Number> > cur_node=top_ptr; cur_node != NULL; cur_node=cur_node->get_next()){
		if(cur_node->get_isDrop()) cur_node=cur_node->get_next();
		else{
			 ++i;
			if(i==1) (*op_1)=cur_node->get_val();
			if(i==2) (*op_2)=cur_node->get_val();
		}
	}
	
}

//BEGIN FUNCTION DECLARATIONS (for main)
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
//BEGIN MAIN
int main (int argc, char *argv[]) {
	bool showTop, stackChanged;
	myStack<double> undo_stack;

	while (!cin.eof()) {
		showTop = true;
		stackChanged = true;
		if (interactive) cout << "> ";

		string s;
		getline(cin, s);
		linenum++;

		double x;
		if (string_to_number(s, x) == 0) {
			undo_stack.undo_op_push(s,x);
		} else if (s == "") {
			// do nothing
		} else if (s=="+" || s=="-" || s=="*" || s=="/" ) {
		// arithmetic operations
			if(undo_stack.size() < 2) {
				cout << "error: stack underflow on line " << linenum << endl;
				showTop = false;
				stackChanged = false;
			} else {
				double num1, num2;
				undo_stack.find_2_nums(&num2,&num1);

				x = calcResult(s,num1,num2);
				undo_stack.undo_op_push(s,x);
			}
		} else if (str2lower(s) == "drop") {
		// drop
			if(undo_stack.size() < 1) {
				cout << "error: stack underflow on line " << linenum << endl;
				showTop = false;
				stackChanged = false;
			}else{
				undo_stack.undo_op_push(s,x);
			}
		} else if (str2lower(s) == "swap") {
		// swap
			if(undo_stack.size() < 2) {
				cout << "error: stack underflow on line " << linenum << endl;
				showTop = false;
				stackChanged = false;
			}else{
					undo_stack.undo_op_push(s,x);
			}
		} else if (str2lower(s) == "undo") {
		// undo
			if(undo_stack.empty()){
				error("cannot undo");
				showTop = false;
				stackChanged = false;
			} else {
				undo_stack.undo_op_pop();
				if(undo_stack.empty()){
					showTop = false;
				}
			}
		} else {
			error("couldn't understand input");
			showTop = false;
			stackChanged = false;
		}
		// if there has not been an error outputted and the stack isnt empty and cin!=eof, show the top element
		if (showTop && !cin.eof() && undo_stack.size()>0) {
			cout << undo_stack.top() << endl;
		}
/*
 * This blurb commented out was used for debugging purposes to print out the stack
for(shared_ptr<Node<double> > i=undo_stack.top_ptr; i!=NULL; i=i->get_next()){
	cout<<i->get_val()<<" -> ";
	if(i->get_next()!=NULL) cout<<i->get_next()->get_val();
	cout<<endl;
}
*/
	}
}
