#pragma once

#include <regex>
#include <iostream>
#include <fstream>
#include <string>

class Parser {
public:
	Parser(std::ifstream* file, std::string expr);
	std::string next_word();
private:
	std::ifstream* infile;
	std::regex word_regex;
};