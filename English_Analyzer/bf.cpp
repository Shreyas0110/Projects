#include "bf.h"
#include "bs.h"
#include "salts.h"
#include "speck.h"
#include <iostream>
using namespace std;

BloomFilter::BloomFilter(uint32_t size): primary{SALT_PRIMARY_LO, SALT_PRIMARY_HI},
	secondary{ SALT_SECONDARY_LO, SALT_SECONDARY_HI }, 
	tertiary{ SALT_TERTIARY_LO, SALT_TERTIARY_HI }, filter(size), sz{size} {}

BloomFilter::~BloomFilter(){}

uint32_t BloomFilter::size() {return filter.size();}

void BloomFilter::insert(string word) {
	uint32_t i = speckhash(primary, &word[0])%sz;
	filter.set_bit(i);
	i = speckhash(secondary, &word[0]) % sz;
	filter.set_bit(i);
	i = speckhash(tertiary, &word[0]) %sz;
	filter.set_bit(i);
}

bool BloomFilter::probe(string word) {
	uint32_t i = speckhash(primary, &word[0]) % sz;
	if (!filter.get_bit(i)) return false;
	i = speckhash(secondary, &word[0]) % sz;
	if(!filter.get_bit(i)) return false;
	i = speckhash(tertiary, &word[0]) % sz;
	if (!filter.get_bit(i)) return false;
	return true;
}

void BloomFilter::print() {
	cout << "primary " << primary<<endl;
	cout << "secondary " << secondary <<endl;
	cout << "tertiary " << tertiary << endl;
	filter.print();
}