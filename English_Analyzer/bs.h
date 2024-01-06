#pragma once

#include <cstdint>

class BitSet {
public:
	BitSet(std::uint32_t l);
	~BitSet();
	std::uint32_t size();
	bool set_bit(std::uint32_t i);
	bool clr_bit(std::uint32_t i);
	bool get_bit(std::uint32_t i);
	void print();
private:
	std::uint32_t length;
	std::uint8_t* vector;
};