#include "scriptLine.h"
using namespace std;

ScriptLine::ScriptLine() 
{
	characterImages = vector<CharacterImage *>();
	backgroundImages = vector<BackgroundImage* >();
	textboxImage = new TextboxImage();
}

ScriptLine::~ScriptLine()
{
	for (auto c : characterImages)
	{
		if (c != nullptr) delete c;
	}
	if (textboxImage != nullptr) delete textboxImage;
}

void ScriptLine::setCharacterRotation(const string& name, const string& expression, bool clockwise, float degree)
{
	for (auto c : characterImages)
	{
		if (c->getName() == name)
		{
			c->changeExpression(expression);
			c->setRotation(clockwise, degree);
			return;
		}
	}
}

void ScriptLine::setBackgroundRotation(const string& name, const string& expression, bool clockwise, float degree)
{
	for (auto c : backgroundImages)
	{
		if (c->getName() == name)
		{
			c->changeExpression(expression);
			c->setRotation(clockwise, degree);
			return;
		}
	}
}

void ScriptLine::setCharacterRotationRel(const string& name, const string& expression, bool clockwise, float degree)
{
	for (auto c : characterImages)
	{
		if (c->getName() == name)
		{
			c->changeExpression(expression);
			c->rotate(clockwise, degree);
			return;
		}
	}
}

void ScriptLine::setBackgroundRotationRel(const string& name, const string& expression, bool clockwise, float degree)
{
	for (auto c : backgroundImages)
	{
		if (c->getName() == name)
		{
			c->changeExpression(expression);
			c->rotate(clockwise, degree);
			return;
		}
	}
}

void ScriptLine::setBackgroundZoom(const string& name, const string& expression, float xScale, float yScale)
{
	for (auto c : backgroundImages)
	{
		if (c->getName() == name)
		{
			c->changeExpression(expression);
			c->setScale(xScale, yScale);
			return;
		}
	}
}

void ScriptLine::setCharacterZoom(const string& name, const string& expression, float xScale, float yScale)
{
	for (auto c : characterImages)
	{
		if (c->getName() == name)
		{
			c->changeExpression(expression);
			c->setScale(xScale, yScale);
			return;
		}
	}
}

void ScriptLine::setCharacter(const string& name, const string& expression, float xPos, float yPos, float fadeTime)
{
	// search if the character already exists
	for (auto c : characterImages)
	{
		if (c->getName() == name)
		{
			c->changeExpression(expression, fadeTime);
			c->setPosition(xPos + (c->getLocalBoundary().width / 2.0f), yPos + (c->getLocalBoundary().height / 2.0f));
			return;
		}
	}

	// otherwise add a new character
	CharacterImage* c = new CharacterImage(name, expression, xPos, yPos);
	c->setPosition(xPos + (c->getLocalBoundary().width / 2.0f), yPos + (c->getLocalBoundary().height / 2.0f));
	characterImages.push_back(c);
}

void ScriptLine::setBackground(const string& name, const string& timeOfTheDay, float xPos, float yPos, float fadeTime)
{
	// search if the character already exists
	for (auto c : backgroundImages)
	{
		if (c->getName() == name)
		{
			c->changeExpression(timeOfTheDay, fadeTime);
			c->setPosition(xPos + (c->getLocalBoundary().width / 2.0f), yPos + (c->getLocalBoundary().height / 2.0f));
			return;
		}
	}

	// otherwise add a new character
	BackgroundImage* c = new BackgroundImage(name, timeOfTheDay, xPos, yPos);
	c->setPosition(xPos + (c->getLocalBoundary().width / 2.0f), yPos + (c->getLocalBoundary().height / 2.0f));
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

void ScriptLine::setDialogue(const string& displayname, const string& str)
{
	hideTextbox = false;
	dialogue = addAllNewLines(str, 70);
	textboxImage->setTextboxColor(GLOBAL->getTextboxColor(displayname));

	name = displayname;
	if (UTILITY->toLower(name) == "player")
	{
		name = GLOBAL->PlayerName;
	}

	textboxImage->setDisplay(name, dialogue);
}

void ScriptLine::changeCharacterPosition(const string & name, float xPos, float yPos)
{
	for (auto c : characterImages)
	{
		if (c->getName() == name)
		{
			c->setPosition(xPos + (c->getLocalBoundary().width / 2.0f), yPos + (c->getLocalBoundary().height / 2.0f));
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

