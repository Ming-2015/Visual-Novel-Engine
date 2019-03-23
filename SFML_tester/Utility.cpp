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
	string b = toLower(s);
	if (b == "true" || b == "t")
		return true;
	else if (b == "false" || b == "f")
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
	tmp = trim(tmp);
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

void Utility::writeToBinaryFile(ofstream & file, const std::string& str) const
{
	if (!file)
	{
		LOGGER->Log("Utility", "Unable to write to file!");
		return;
	}

	// null terminated string
	file.write(str.c_str(), str.size() + 1);
}

std::string Utility::readFromBinaryFile(ifstream & file) const
{
	if (!file || file.eof())
	{
		LOGGER->Log("Utility", "Unable to read from file!");
		return NULL;
	}

	std::string line;
	std::getline(file, line, '\0');
	return line;
}

void Utility::writeVectorToBinaryFile(ofstream & file, const std::vector<std::string> strs) const
{
	int size = strs.size();
	file.write(reinterpret_cast<const char*> (&size), sizeof(size));
	for (int i = 0; i < size; i++)
	{
		writeToBinaryFile(file, strs[i]);
	}
}

std::vector<std::string> Utility::readVectorFromBinaryFile(ifstream & file) const
{
	std::vector<std::string> ret;
	int size;
	file.read(reinterpret_cast<char*> (&size), sizeof(size));
	for (int i = 0; i < size; i++)
	{
		ret.push_back(readFromBinaryFile(file));
	}
	return ret;
}

sf::Image Utility::imageDownscale(const sf::Image & original, unsigned int xFactor, unsigned int yFactor) const
{
	sf::Image newImage;

	unsigned int width = original.getSize().x;
	unsigned int height = original.getSize().y;
	unsigned int thumbWidth = width / xFactor;
	unsigned int thumbHeight = height / yFactor;

	newImage.create(thumbWidth, thumbHeight);
	unsigned int areaSize = xFactor * yFactor;

	for (unsigned int new_y = 0; new_y < thumbHeight; new_y++) // y on output
	{
		for (unsigned int new_x = 0; new_x < thumbWidth; new_x++)
		{
			unsigned int r = 0, g = 0, b = 0, a = 0;
			
			for (unsigned int original_y = new_y * yFactor; original_y < new_y * yFactor + yFactor; original_y++)
			{
				for (unsigned int original_x = new_x * xFactor; original_x < new_x * xFactor + xFactor; original_x++)
				{
					r += original.getPixel(original_x, original_y).r;
					g += original.getPixel(original_x, original_y).g;
					b += original.getPixel(original_x, original_y).b;
					a += original.getPixel(original_x, original_y).a;
				}
			}

			r /= areaSize;
			g /= areaSize;
			b /= areaSize;
			a /= areaSize;

			newImage.setPixel(new_x, new_y, sf::Color(r, g, b, a));
		}
	}

	return newImage;
}

bool Utility::checkFileExist(const std::string & filepath) const
{
	struct stat buffer;
	return (stat(filepath.c_str(), &buffer) == 0);
}

sf::Image Utility::getScreenshot(const sf::RenderWindow & window, unsigned int xScale, unsigned int yScale) const
{
	int x = xScale, y = yScale;
	if (xScale < 1 || yScale < 1)
	{
		LOGGER->Log("Utility", "Unable to get screenshot of scale 0. Returning default size.");
		x = 1;
		y = 1;
	}

	sf::Vector2u windowSize = window.getSize();
	sf::Texture texture;
	texture.create(windowSize.x, windowSize.y);
	texture.update(window);
	sf::Image screenshot = texture.copyToImage();
	screenshot = UTILITY->imageDownscale(screenshot, x, y);

	return screenshot;
}

std::string Utility::replaceAllSubstrings(const std::string& line, const std::string& start, const std::string& end) const
{
	std::string ret = line;

	// startAll finding the first to replace
	int index = ret.find(start);
	while (index != std::string::npos)
	{
		ret.replace(index, start.length(), end);
		index += end.length();

		index = ret.find(start, index);
	}

	return ret;
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

