#include "anode.h"
#include <iostream>
#include <format>
#include <cmath>
#include <iomanip>
using namespace std;

AuthorNode::AuthorNode(std::string author, double dist):author{author}, distance{dist} {}
void AuthorNode::print() {
	cout << fixed << showpoint;
	cout << setprecision(16);
	cout<<"author: " << author << "\tdistance: " << abs(distance) << endl;
}
string AuthorNode::getAuthor() { return author; }
double AuthorNode::getDistance() { return distance; }

bool AuthorNodeCmp::operator()(AuthorNode node1, AuthorNode node2) {
	if (node1.getDistance() <= node2.getDistance()) return false;
	return true;
}