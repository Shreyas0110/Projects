#include "text.h"
#include <cmath>
#include "parser.h"

using namespace std;

Text::Text(ifstream* infile, Text* noise): ht{*(new HashTable((uint32_t)pow(2,19)))}, 
bf{*(new BloomFilter((uint32_t)pow(2,20)))}, word_count{0} {
	Parser* p = new Parser(infile, "[a-zA-Z'-,\"]+");
	string word;
	if (noise == nullptr) {
		for (word = p->next_word(); !word.empty() && word_count<noiselimit; word = p->next_word()) {
			if (!isalpha(word.back())) //see if better way to do this
				word.pop_back();
			if (!isalpha(word[0]))
				word.erase(0, 1);
			word[0] = tolower(word[0]); //assume only first letter can be capitalized
			ht.insert(word);
			bf.insert(word);
			++word_count;
		}
	}
	else{
		for (word = p->next_word(); !word.empty(); word = p->next_word()) {
			if (!isalpha(word.back())) //see if better way to do this
				word.pop_back();
			if (!isalpha(word[0]))
				word.erase(0, 1);
			word[0] = tolower(word[0]);
			if(!noise->contains(word) && !word.empty()){
				ht.insert(word);
				bf.insert(word);
				++word_count;
			}
		}
	}
	delete p;
}

Text::~Text() {
	delete &ht;
	delete &bf;
}

bool Text::contains(string word) {
	if (bf.probe(word) && ht.lookup(word) != nullptr)
		return true;
	return false;
}
/*
* for man and euc - iterate through this text, delete elements from t2 
and then reiterate through t2, deleting it at the end*/
double Text::dist(Text* t2, Metric metric) {
	double distance = 0;
	double v1, v2 = 0;
	HashTableIter* hti = new HashTableIter(ht);
	Node* n;
	switch (metric) {
	case MANHATTAN:
		while ((n = hti->next()) != nullptr) {
			v2 = 0;
			v1 = n->getCount() / (double)word_count;
			double value;
			if ((value = t2->frequency_delete(n->getWord())) != NULL) { //bad coding practice i think
				v2 = value / (double)t2->count();
			}
			distance += abs(v1 - v2);
		}
		delete hti;
		//t2->print();
		
		hti = new HashTableIter(t2->ht);
		while ((n = hti->next()) != nullptr) { //second loop to find unique words in 2nd ht
			v2 = 0;
			v1 = n->getCount() / (double)t2->count();		
			/*Due to chaining, the method of deleting words in common is prone to errors, as chained words will not be found
			* to remedy this, we will find all of the chained words, and, if they are in common, subtract their weight
			*/
			double value;
			if ((value = this->frequency(n->getWord())) != NULL) {
				v2 = value / (double)word_count;
				distance -= v2;
				distance += abs(v1 - v2);
			}
			else { distance += v1; }
		}
		delete hti;
		delete t2;
		return distance;

	case EUCLIDEAN:
		while ((n = hti->next()) != nullptr) {
			v2 = 0;
			v1 = n->getCount() / (double)word_count;
			double value;
			if ((value = t2->frequency_delete(n->getWord())) != NULL) { //bad coding practice i think
				v2 = value / (double)t2->count();
			}
			double temp = abs(v1 - v2);
			distance += pow(temp, 2);
		}
		delete hti;
		hti = new HashTableIter(t2->ht);
		while ((n = hti->next()) != nullptr) { //second loop to find unique words in 2nd ht
			v1 = n->getCount() / (double)t2->count();
			/*Due to chaining, the method of deleting words in common is prone to errors, as chained words will not be found
			* to remedy this, we will find all of the chained words, and, if they are in common, subtract their weight
			*/
			double value;
			if ((value = this->frequency(n->getWord())) != NULL) {
				v2 = value / (double)word_count;
				distance -= pow(v2, 2);
				double temp = abs(v1 - v2);
				distance += pow(temp, 2);
			}
			else{distance += pow(v1, 2);}
		}
		delete hti;
		distance = sqrt(abs(distance));
		delete t2;
		return distance;

	case COSINE:
		while((n = hti->next()) != nullptr) {
			v2 = 0;
			v1 = n->getCount() / (double)word_count;
			double value;
			if ((value = t2->frequency(n->getWord())) != NULL) { //bad coding practice i think
				v2 = value / (double)t2->count();
			}
			else {continue;} //unique to first one, product will be 0, so skip remainder
			distance += v1*v2;
		}
		delete hti;
		distance = 1.0 - distance;
		cout << "\ndistance is: \t" << distance << endl;
		delete t2;
		return distance;
	}
}

double Text::frequency(string word) {
	Node* n;
	if (bf.probe(word) && (n = ht.lookup(word)) != nullptr)
		return n->getCount();
	return NULL;
}

double Text::frequency_delete(string word) {
	Node* n;
	if (bf.probe(word) && (n = ht.remove(word)) != nullptr){
		double result = n->getCount();
		delete n;
		return result;
	}
	return NULL;
}

uint32_t Text::count() {
	return word_count;
}

void Text::print() {
	ht.print();
	cout << "Total Words:\t" << word_count << "\n\n";
}