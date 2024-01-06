#pragma once
#include "metric.h"
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include "ht.h"
#include "bf.h"

extern std::uint32_t noiselimit; // Number of noise words to filter out.

class Text {
public:
	Text(std::ifstream* infile, Text* noise);
	~Text();
	double dist(Text* text2, Metric metric);
	double frequency(std::string word);
	double frequency_delete(std::string word);
	bool contains(std::string word);
	void print();
	std::uint32_t count();
	HashTable& ht;
private:
	BloomFilter& bf;
	std::uint32_t word_count;
};
