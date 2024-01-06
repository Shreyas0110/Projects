#pragma once

#include <cstdint>
#include <string>

class Node {
public:
	Node(std::string word);
	~Node();
	void print();
	std::string getWord();
	std::uint32_t getCount();
	void addCount();
private:
	std::string word;
	std::uint32_t count;
};