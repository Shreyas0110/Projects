#pragma once

#include <cstdint>
#include <string>

class AuthorNode {
public:
	AuthorNode(std::string author, double dist);
	void print();
	std::string getAuthor();
	double getDistance();
private:
	std::string author;
	double distance;
};

class AuthorNodeCmp {
public:
	bool operator()(AuthorNode node1, AuthorNode node2);
};