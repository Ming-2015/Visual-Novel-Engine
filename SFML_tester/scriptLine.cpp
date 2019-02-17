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

	for (auto m : bgm)
	{
		m->stop();
		delete m;
	}

	for (auto m : voices)
	{
		m->stop();
		delete m;
	}

	for (auto m : sfx)
	{
		m->stop();
		delete m;
	}
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

void ScriptLine::moveBackgroundRel(const string& name, const string& expression, float x1, float y1)
{
	for (auto c : backgroundImages)
	{
		if (c->getName() == name)
		{
			c->changeExpression(expression);
			c->move(x1, y1);
			return;
		}
	}
}

void ScriptLine::moveCharacterRel(const string& name, const string& expression, float x1, float y1)
{
	for (auto c : characterImages)
	{
		if (c->getName() == name)
		{
			c->changeExpression(expression);
			c->move(x1, y1);
			return;
		}
	}
}

float ScriptLine::getBackgroundBeginScaleX(const string& name, const string& expression)
{
	for (auto c : backgroundImages)
	{
		if (c->getName() == name)
		{
			return c->getScale().x;
		}
	}
}

float ScriptLine::getBackgroundBeginScaleY(const string& name, const string& expression)
{
	for (auto c : backgroundImages)
	{
		if (c->getName() == name)
		{
			return c->getScale().y;
		}
	}
}

float ScriptLine::getCharacterBeginScaleY(const string& name, const string& expression)
{
	for (auto c : characterImages)
	{
		if (c->getName() == name)
		{
			return c->getScale().y;
		}
	}
}

float ScriptLine::getCharacterBeginScaleX(const string& name, const string& expression)
{
	for (auto c : characterImages)
	{
		if (c->getName() == name)
		{
			return c->getScale().x;
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

sf::Music* ScriptLine::setBgm(const string & groupname, const string & filename, bool clearOthers, bool repeat, float volume)
{
	if (clearOthers)
	{
		for (int i = 0; i < bgm.size(); i++)
		{
			bgm[i]->stop();
			delete bgm[i];
		}
		bgm.clear();
	}

	string f = "sound/" + groupname + "/" + filename;
	sf::Music* music = new sf::Music();
	if (!music->openFromFile(f))
	{
		string err = "Unable to load bgm file: " + f;
		LOGGER->Log("ScriptLine", err);
		delete music;
		return nullptr;
	}

	bgm.push_back(music);
	music->setLoop(repeat);
	music->setVolume(volume * CONFIG->bgmVolume * CONFIG->masterVolume * 100.f);
	music->play();
	return music;
}

sf::Music* ScriptLine::setVoice(const string & groupname, const string & filename, bool clearOthers, bool repeat, float volume)
{
	if (clearOthers)
	{
		for (int i = 0; i < bgm.size(); i++)
		{
			voices[i]->stop();
			delete voices[i];
		}
		voices.clear();
	}

	string f = "sound/" + groupname + "/" + filename;
	sf::Music* music = new sf::Music();
	if (!music->openFromFile(f))
	{
		string err = "Unable to load music file: " + f;
		LOGGER->Log("ScriptLine", err);
		delete music;
		return nullptr;
	}

	voices.push_back(music);
	music->setLoop(repeat);
	music->setVolume(volume * CONFIG->voiceVolume * CONFIG->masterVolume * 100.f);
	music->play();
	return music;
}

sf::Music* ScriptLine::setSfx(const string & groupname, const string & filename, bool clearOthers, bool repeat, float volume)
{
	if (clearOthers)
	{
		for (int i = 0; i < bgm.size(); i++)
		{
			sfx[i]->stop();
			delete sfx[i];
		}
		sfx.clear();
	}

	string f = "sound/" + groupname + "/" + filename;
	sf::Music* music = new sf::Music();
	if (!music->openFromFile(f))
	{
		string err = "Unable to load music file: " + f;
		LOGGER->Log("ScriptLine", err);
		delete music;
		return nullptr;
	}

	sfx.push_back(music);
	music->setLoop(repeat);
	music->setVolume(volume * CONFIG->sfxVolume * CONFIG->masterVolume * 100.f);
	music->play();
	return music;
}

void ScriptLine::setBgmVolume(float volume)
{
	for (auto m : bgm)
	{
		if (m != nullptr) m->setVolume(volume * CONFIG->masterVolume * CONFIG->bgmVolume * 100.f);
	}
}

void ScriptLine::setVoiceVolume(float volume)
{
	for (auto m : voices)
	{
		if (m != nullptr) m->setVolume(volume * CONFIG->masterVolume * CONFIG->voiceVolume * 100.f);
	}
}

void ScriptLine::setSfxVolume(float volume)
{
	for (auto m : sfx)
	{
		if (m != nullptr) m->setVolume(volume * CONFIG->masterVolume * CONFIG->sfxVolume * 100.f);
	}
}

void ScriptLine::setBgmVolume(sf::Music * m, float volume)
{
	if (m != nullptr) m->setVolume(volume * CONFIG->masterVolume * CONFIG->bgmVolume * 100.f);
}

void ScriptLine::setVoiceVolume(sf::Music * m, float volume)
{
	if (m != nullptr) m->setVolume(volume * CONFIG->masterVolume * CONFIG->voiceVolume * 100.f);
}

void ScriptLine::setSfxVolume(sf::Music * m, float volume)
{
	if (m != nullptr) m->setVolume(volume * CONFIG->masterVolume * CONFIG->sfxVolume * 100.f);
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

