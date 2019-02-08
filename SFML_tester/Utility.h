#pragma once
#ifndef Utility_h
#define Utility_h
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
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

	// split a string off a char delimiter
	std::vector<std::string> split(const std::string& s, char delimiter);

	// skip n number of lines in the file stream
	bool skipFileLines(ifstream& file, unsigned int n);

	// skip n bytes (or ascii chars) in the file stream
	bool skipFileBytes(ifstream& file, unsigned int n);

	static Utility* GetUtility();
	static void CleanUp();

private:
	Utility();
	static Utility * util_ptr;
};

#endif
