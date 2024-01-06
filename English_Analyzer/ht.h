#pragma once

#include "node.h"
#include <cstdint>

class HashTable {
public:
	HashTable(std::uint32_t);
	~HashTable();
	std::uint32_t size();
	Node* lookup(std::string word);
	Node* insert(std::string word);
	Node* remove(std::string word);
	Node* operator[](int i);
	void print();

private:
	Node** slots;
	std::uint32_t sz;
	std::uint64_t salt[2];
};

class HashTableIter {
private:
	HashTable& table;
	uint32_t slot;
public:
	HashTableIter(HashTable& ht);
	Node* next();
};