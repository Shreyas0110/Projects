#include"parser.h"
using namespace std;

Parser::Parser(ifstream* file, string expr): infile{file}, word_regex(expr) {}

string Parser::next_word() {
	string word;
	while (*infile >> word) {
		if (regex_match(word, word_regex))
			return word;
	}
	return "";
}