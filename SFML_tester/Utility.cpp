#include "Utility.h"
#include "charPic.h"
#include "scriptLine.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

Utility::Utility()
{

}

static std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

bool Utility::str2bool(string s) {
	if (s == "true")
		return true;
	else if (s == "false")
		return false;
	throw exception("bad string");
}

string Utility::bool2str(bool b) {
	if (b == true)
		return "true";
	else if (b == false)
		return "false";
	throw exception("bad boolean");
}
