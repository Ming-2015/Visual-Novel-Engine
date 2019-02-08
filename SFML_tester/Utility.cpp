#include "Utility.h"
using namespace std;

Utility * Utility::util_ptr = nullptr;

Utility::Utility()
{
	
}

std::vector<std::string> Utility::split(const std::string& s, char delimiter)
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

string Utility::trim(const std::string& src)
{
	if (src.empty())
		return "";

	int i = 0; // first index 
	int j = src.size() - 1; // last index of src

	while (i < j && isspace(src[i]))
		i++;

	while (j > i && isspace(src[j]))
		j--;

	return src.substr(i, j - i + 1);
}

Utility* Utility::GetUtility()
{
	if (util_ptr == nullptr)
		util_ptr = new Utility();
	return util_ptr;
}

void Utility::CleanUp()
{
	if (util_ptr)
	{
		delete util_ptr;
	}
}
