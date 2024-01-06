#pragma once

#include <cstdint>
#include <string>
#include "bs.h"

class BloomFilter {
public:
	BloomFilter(std::uint32_t size);
	~BloomFilter();
	uint32_t size();
	void insert(std::string oldspeak);
	bool probe(std::string oldspeak);
	void print();
private:
	uint64_t primary[2];
	uint64_t secondary[2];
	uint64_t tertiary[2];
	BitSet filter;
	uint32_t sz;
};