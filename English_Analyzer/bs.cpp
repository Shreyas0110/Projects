#include "bs.h"
#include <iostream>
#include <format>
using namespace std;

BitSet::BitSet(uint32_t l): length{l}, vector{new uint8_t[(l / 8)+(l % 8 != 0)]()} {}

BitSet::~BitSet() { delete vector; }

uint32_t BitSet::size() { return length; }

bool BitSet::set_bit(uint32_t i) {
	if (i >= length) return false;
	vector[i / 8] |= (1 << i % 8);
	return true;
}

bool BitSet::clr_bit(uint32_t i) {
	if (i >= length) return false;
	vector[i / 8] &= ~(1 << i % 8);
	return true;
}

bool BitSet::get_bit(uint32_t i) {
	if (i >= length) return false;
	return vector[i / 8] & (1 << i % 8);
}

void BitSet::print() {
	auto count = 0;
	for (auto i = 0; i < (length / 8) + (length % 8 != 0); i++){
		for (auto j = 0; j < 8 && count < length; j++){
			count++;
			(vector[i] & (1 << j))? cout<<'1':cout<<'0';
		}
	}
	cout << '\n';
}