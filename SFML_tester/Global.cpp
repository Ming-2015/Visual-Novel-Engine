#include "Global.h"

Global * Global::global_ptr = nullptr;

sf::Color Global::getTextboxColor(const std::string& name)
{
	std::string n = UTILITY->toLower(name);
	if (textBoxColors.find(n) != textBoxColors.end())
	{
		return textBoxColors[n];
	}
	else
	{
		return textBoxColors["default"];
	}
}

Global * Global::GetGlobal()
{
	if (global_ptr == nullptr)
		global_ptr = new Global();
	return global_ptr;
}

void Global::CleanUp()
{
	if (global_ptr != nullptr)
		delete global_ptr;
}

Global::~Global()
{
}

void Global::initTextboxColors(const std::string& filename)
{
	ifstream file;
	file.open(filename);
	if (!file)
	{
		std::string err = "Unable to access textbox colors of characters: " + filename;
		LOGGER->Log("Global", err);
	}
	else
	{
		std::string line;
		std::vector < std::string > tokens;
		std::string name;
		float r, g, b;
		while (std::getline(file, line))
		{
			tokens = UTILITY->split(line, '|');
			tokens = UTILITY->trim(tokens);

			if (tokens.size() < 4)
			{
				std::string err = "Invalid number of arguments in " + filename 
					+ " - too many arguments: " + to_string(tokens.size());
				LOGGER->Log("Global", err);
				continue;
			}

			name = UTILITY->toLower(tokens[0]);
			try
			{
				r = stof(tokens[1]);
				g = stof(tokens[2]);
				b = stof(tokens[3]);
			}
			catch (exception e)
			{
				std::string err = "Invalid float arguments in " + filename;
				LOGGER->Log("Global", err);
				continue;
			}

			textBoxColors[name] = sf::Color(r, g, b, 255.f);
		}
	}

	if (textBoxColors.find("default") == textBoxColors.end())
	{
		textBoxColors["default"] = sf::Color(100, 255, 100, 255);
	}
}

Global::Global()
{
	initTextboxColors(TextBoxColorLocation);
}