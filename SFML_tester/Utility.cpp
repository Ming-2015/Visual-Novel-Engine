#include "Utility.h"
using namespace std;

Utility * Utility::util_ptr = nullptr;

Utility::Utility()
{
	
}

std::string Utility::parseExpression(const std::string& str, char c)
{
	bool prepend = str[0] == c;
	bool append = str[str.length() - 1] == c;

	std::vector<std::string> tokens = split(str, c);

	std::string parsedExpression = trim(tokens[0]);

	for (int i = 1; i < tokens.size(); i++)
	{
		parsedExpression += " " + string(1, c) + " " + tokens[i];
	}

	//if (prepend) parsedExpression = string(1, c) + parsedExpression;
	if (append) parsedExpression = parsedExpression + " " + string(1, c);

	return trim(parsedExpression);
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
	if (s == "true" || s== "TRUE")
		return true;
	else if (s == "false" || s == "FALSE")
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

vector<string> Utility::trim(const vector<string>& src)
{
	vector<string> tmp = vector<string>();
	for (string s : src)
	{
		tmp.push_back(trim(s));
	}
	return tmp;
}

bool Utility::skipFileLines(ifstream & file, unsigned int n)
{
	std::string line;
	for (int i = 0; i < n; i++)
	{
		// return false if file reaches the end
		if (file.eof() || !file) return false;

		// return false if an error is thrown
		if (!file.ignore(numeric_limits<streamsize>::max(), file.widen('\n')))
			return false;
	}

	// return true after skipping the correct number of lines
	return true;
}

bool Utility::skipFileBytes(ifstream & file, unsigned int n)
{
	return (bool) file.ignore(n, ios::eofbit);
}

unsigned int Utility::findLastOf(string text, char c, unsigned int n)
{
	int last = n - 1;
	if (n > text.length()) n = text.length() - 1;

	for (int i = last; i >= 0; i--)
	{
		if (text[i] == c) return i;
	}

	return 0;
}

std::string Utility::cutLine(const std::string & line, const std::string & symbol)
{
	std::string tmp = line;
	auto pos = tmp.find(symbol);
	if (pos != std::string::npos)
	{
		tmp = tmp.substr(0, pos);
	}
	return tmp;
}

std::string Utility::toUpper(const std::string& str)
{
	std::string tmp = str;
	for (auto & c : tmp) c = toupper(c);
	return tmp;
}

std::string Utility::toLower(const std::string & str)
{
	std::string tmp = str;
	for (auto & c : tmp) c = tolower(c);
	return tmp;
}

bool Utility::evaluateFlagExpression(const std::set<std::string>& flags, const std::string & flagExpression)
{
	bool expression = false;

	// add empty spaces between the expression and operator
	std::string parsedExpression = flagExpression;
	parsedExpression = parseExpression(parsedExpression, '&');
	parsedExpression = parseExpression(parsedExpression, '^');
	parsedExpression = parseExpression(parsedExpression, '!');
	parsedExpression = parseExpression(parsedExpression, '(');
	parsedExpression = parseExpression(parsedExpression, ')');

	// separate all the items into tokens
	std::vector<std::string> tmp = split(parsedExpression, ' ');
	std::vector<std::string> tokens;
	for (int i = 0; i < tmp.size(); i++)
	{
		if (tmp[i] != "")
		{
			tokens.push_back(tmp[i]);
		}
	}

	// initialize stack
	std::stack<std::string> opstack;
	std::vector<std::string> postfix;

	for (int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] == "&" || tokens[i] == "^" || tokens[i] == "!")
		{
			// NOTE: might need to consider precedence
			opstack.push(tokens[i]);
		}

		else if (tokens[i] == "(")
		{
			opstack.push(tokens[i]);
		}

		else if (tokens[i] == ")")
		{
			while (opstack.top() != "(")
			{
				postfix.push_back(opstack.top());
				opstack.pop();
			}

			// remove the "(" 
			opstack.pop();
		}

		else
		{
			postfix.push_back(tokens[i]);
		}
	}

	while (!opstack.empty())
	{
		postfix.push_back(opstack.top());
		opstack.pop();
	}

	std::stack< bool > evaluatedExpression;
	for (int i = 0; i < postfix.size(); i++)
	{
		if (postfix[i] == "&")
		{
			bool left = evaluatedExpression.top();
			evaluatedExpression.pop();
			bool right = evaluatedExpression.top();
			evaluatedExpression.pop();
			evaluatedExpression.push(left && right);
		}
		else if (postfix[i] == "^")
		{
			bool left = evaluatedExpression.top();
			evaluatedExpression.pop();
			bool right = evaluatedExpression.top();
			evaluatedExpression.pop();
			evaluatedExpression.push(left || right);
		}
		else if (postfix[i] == "!")
		{
			bool operand = evaluatedExpression.top();
			evaluatedExpression.pop();
			evaluatedExpression.push(!operand);
		}
		else
		{
			evaluatedExpression.push(flags.find(postfix[i]) != flags.end());
		}
	}

	if (evaluatedExpression.size() != 1)
	{
		std::string err = "Failed to properly parse flag expression: " + flagExpression;
		LOGGER->Log("Utility", err);
		return false;
	}

	expression = evaluatedExpression.top();

	return expression;
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


std::string Utility::addNewLineToPrevWord(std::string str, unsigned int pos)
{
	unsigned int found = UTILITY->findLastOf(str, ' ', pos);
	return str.substr(0, found + 1) + "\n"
		+ str.substr(found + 1, str.length() - found + 1);
}

std::string Utility::addAllNewLines(string str, unsigned int lineLength)
{
	string tmp = str;
	int currentChar = lineLength;

	while (currentChar < tmp.length())
	{
		tmp = addNewLineToPrevWord(tmp, currentChar);
		currentChar += lineLength + 1;
	}

	return tmp;
}

