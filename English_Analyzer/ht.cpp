#include "ht.h"
#include "speck.h"
#include "salts.h"
#include <iterator>
#include <string>
#include <iostream>
using namespace std;

HashTable::HashTable(uint32_t size): sz{size}, slots{new Node*[size]},
						salt{SALT_HASHTABLE_LO, SALT_HASHTABLE_HI}{
	memset(slots, 0, size * sizeof(slots));
}

HashTable::~HashTable() {
	for (auto i = 0; i < sz; i++) {
		delete slots[i];
	}
	delete slots;
}

uint32_t HashTable::size() {return sz;}

Node* HashTable::lookup(string word) {
	uint32_t slot = speckhash(salt, &word[0]) % sz;
	while (slot < sz && slots[slot] != nullptr) {
		if (slots[slot]->getWord() == word) {
			return slots[slot];
		}
		slot++;
	}
	return nullptr;
}

Node* HashTable::insert(string word) {
	uint32_t slot = speckhash(salt, &word[0]) % sz;
	while (slot < sz && slots[slot] != nullptr) {
		if (slots[slot]->getWord() == word) {
			slots[slot]->addCount();
			return slots[slot];
		}
		slot++;
	}
	if (slot >= sz) return nullptr;
	if (slots[slot] == nullptr) {
		slots[slot] = new Node(word);
		return slots[slot];
	}
}

void HashTable::print() {
	for (auto i = 0; i < sz; i++) {
		if(slots[i] != nullptr)
			cout <<i<<'\t'<< slots[i]->getWord() << '\t' << slots[i]->getCount() << '\n';
	}
}

Node* HashTable::operator[](int i){
	return slots[i];
}

Node* HashTable::remove(string word) {
	uint32_t slot = speckhash(salt, &word[0]) % sz;
	while (slot < sz && slots[slot] != nullptr) {
		if (slots[slot]->getWord() == word) {
			Node* n = slots[slot];
			slots[slot] = nullptr;
			return n;
		}
		slot++;
	}
	return nullptr;
}


HashTableIter::HashTableIter(HashTable& ht): table{ht}, slot{0}{}
Node* HashTableIter::next() {
	while (slot < table.size() && table[slot] == nullptr){
		slot++;
	}
	if (slot >= table.size())
		return nullptr;
	slot++;
	return table[slot-1];
}
