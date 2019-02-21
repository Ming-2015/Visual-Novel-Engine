#pragma once
#ifndef Utility_h
#define Utility_h
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <SFML/Graphics.hpp>
#include "Logger.h"
using namespace std;

#define UTILITY Utility::GetUtility()

class Utility {
public:
	// convert a string to boolean
	bool str2bool(string);

	// convert a boolean to string
	string bool2str(bool);

	// trim a string of empty spaces at the beginning and ending of a string
	string trim(const std::string& src);

	vector<string> trim(const vector<string>& src);

	// split a string off a char delimiter
	std::vector<std::string> split(const std::string& s, char delimiter);

	// skip n number of lines in the file stream
	bool skipFileLines(ifstream& file, unsigned int n);

	// skip n bytes (or ascii chars) in the file stream
	bool skipFileBytes(ifstream& file, unsigned int n);

	unsigned int findLastOf(string text, char c, unsigned int n);

	std::string cutLine(const std::string& line, const std::string& symbol);

	std::string toUpper(const std::string& str);
	std::string toLower(const std::string& str);

	bool evaluateFlagExpression(const std::set<std::string>& flags, const std::string& flagExpression);

	static Utility* GetUtility();
	static void CleanUp();

	std::string addAllNewLines(std::string str, unsigned int lineLength);
	std::string addNewLineToPrevWord(std::string str, unsigned int pos);

private:
	Utility();
	static Utility * util_ptr;

	std::string parseExpression(const std::string& str, char c);
};

#endif
