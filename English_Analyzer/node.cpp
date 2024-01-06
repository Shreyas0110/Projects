#include "node.h"
#include <iostream>
using namespace std;

Node::Node(string word): word{word}, count{1} {}

Node::~Node() {};

void Node::print() {
	cout << "Word is: " << word << '\t' << "count: " <<count<< endl;
}

string Node::getWord() {
	return word;
}
uint32_t Node::getCount() {
	return count;
}
void Node::addCount() {
	count += 1;
}