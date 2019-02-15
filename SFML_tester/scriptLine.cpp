#include "scriptLine.h"
using namespace std;

ScriptLine::ScriptLine() 
{
	characterImages = vector<CharacterImage *>();
}

ScriptLine::~ScriptLine()
{
	for (auto c : characterImages)
	{
		if (c != nullptr) delete c;
	}
}

void ScriptLine::setCharacter(const string& name, const string& expression, float xPos, float yPos, 
	float fadeTime, float xScale, float yScale, bool clockwise, float angle)
{
	// search if the character already exists
	for (auto c : characterImages)
	{
		if (c->getName() == name)
		{
			c->changeExpression(expression, fadeTime);
			c->setPosition(xPos, yPos);
			c->setScale(xScale, yScale);
			c->setRotation(clockwise, angle);
			return;
		}
	}

	// otherwise add a new character
	CharacterImage* c = new CharacterImage(name, expression, xPos, yPos);
	c->setPosition(xPos, yPos);
	c->setScale(xScale, yScale);
	c->setRotation(clockwise, angle);
	characterImages.push_back(c);
}

void ScriptLine::setBackground(const string& name, const string& timeOfTheDay, float xPos, float yPos, float fadeTime, float xScale, float yScale, bool clockwise, float angle)
{
	// search if the character already exists
	for (auto c : backgroundImages)
	{
		if (c->getName() == name)
		{
			c->changeExpression(timeOfTheDay, fadeTime);
			c->setPosition(xPos, yPos);
			c->setScale(xScale, yScale);
			c->setRotation(clockwise, angle);
			return;
		}
	}

	// otherwise add a new character
	BackgroundImage* c = new BackgroundImage(name, timeOfTheDay, xPos, yPos);
	c->setPosition(xPos, yPos);
	c->setScale(xScale, yScale);
	c->setRotation(clockwise, angle);
	backgroundImages.push_back(c);
}

void ScriptLine::setCharacterAlpha(const string & name, float alpha)
{
	for (auto c : characterImages)
	{
		if (c->getName() == name)
		{
			c->setAlpha(alpha);
			return;
		}
	}
}

void ScriptLine::setBackgroundAlpha(const string & name, float alpha)
{
	for (auto c : backgroundImages)
	{
		if (c->getName() == name)
		{
			c->setAlpha(alpha);
			return;
		}
	}
}

void ScriptLine::removeCharacter(const string & name)
{
	for (auto it = characterImages.begin(); it != characterImages.end(); ++it)
	{
		if ((*it)->getName() == name)
		{
			delete *it;
			characterImages.erase(it);
			return;
		}
	}
}

void ScriptLine::removeBackground(const string & name)
{
	for (auto it = backgroundImages.begin(); it != backgroundImages.end(); ++it)
	{
		if ((*it)->getName() == name)
		{
			delete (*it);
			backgroundImages.erase(it);
			return;
		}
	}
}

void ScriptLine::removeAllCharacters()
{
	for (auto c : characterImages)
	{
		delete c;
	}
	characterImages.clear();
}

void ScriptLine::removeAllBackgrounds()
{
	for (auto c : backgroundImages)
	{
		delete c;
	}
	backgroundImages.clear();
}

void ScriptLine::setDialogue(string str)
{
	dialogue = addAllNewLines(str, 70);
}

void ScriptLine::changeCharacterPosition(const string & name, float xPos, float yPos)
{
	for (auto c : characterImages)
	{
		if (c->getName() == name)
		{
			c->setPosition(xPos, yPos);
			return;
		}
	}
}

std::string ScriptLine::addNewLineToPrevWord(std::string str, unsigned int pos)
{
	unsigned int found = UTILITY->findLastOf(str, ' ', pos);
	return str.substr(0, found + 1) + "\n"
		+ str.substr(found + 1, str.length() - found + 1);
}

std::string ScriptLine::addAllNewLines(string str, unsigned int lineLength)
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

