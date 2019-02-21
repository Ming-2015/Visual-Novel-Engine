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

	for (auto c : choiceImages)
	{
		if (c != nullptr) delete c;
	}

	for (auto m : bgm)
	{
		if (m != nullptr)
		{
			m->stop();
			delete m;
		}
	}

	for (auto m : voices)
	{
		if (m != nullptr)
		{
			m->stop();
			delete m;
		}
	}

	for (auto m : sfx)
	{
		if (m != nullptr)
		{
			m->stop();
			delete m;
		}
	}
}

void ScriptLine::setChoices(const vector<string>& choices, const vector<string>& flags)
{
	isChoice = true;
	if (choices.size() != flags.size())
	{
		LOGGER->Log("ScriptLine", "The number of choices is not matching the number of flags.");
		return;
	}

	for (auto c : choiceImages)
	{
		if (c != nullptr) delete c;
	}
	choiceImages.clear();

	vector<string> tmp;
	for (int i = 0; i < choices.size(); i++)
	{
		tmp.push_back(UTILITY->addAllNewLines(choices[i], 60) );
		ChoiceImage * choiceImage = new ChoiceImage(tmp[i], flags[i], choices.size(), i);
		choiceImages.push_back(choiceImage);
	}
}

void ScriptLine::clearChoices()
{
	for (auto c : choiceImages)
	{
		if (c != nullptr) delete c;
	}
	choiceImages.clear();
	isChoice = false;
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

void ScriptLine::setBackgroundZoom(const string& name, const string& expression, float xScale, float yScale, float xPos, float yPos)
{
	for (auto c : backgroundImages)
	{
		if (c->getName() == name)
		{
			c->changeExpression(expression);
			c->setScale(xScale, yScale, xPos, yPos);
			return;
		}
	}
}

void ScriptLine::setCharacterZoom(const string& name, const string& expression, float xScale, float yScale, float xPos, float yPos)
{
	for (auto c : characterImages)
	{
		if (c->getName() == name)
		{
			c->changeExpression(expression);
			c->setScale(xScale, yScale, xPos, yPos);
			return;
		}
	}
}

void ScriptLine::setCharacterShader(const string& name, const string& shaderSrc)
{
	for (auto c : characterImages)
	{
		if (c->getName() == name)
		{
			c->setShader(shaderSrc);
			return;
		}
	}
}

void ScriptLine::setBackgroundShader(const string& name, const string& shaderSrc)
{
	for (auto c : backgroundImages)
	{
		if (c->getName() == name)
		{
			c->setShader(shaderSrc);
			return;
		}
	}
}

void ScriptLine::tickBackgroundShader(const string& name, bool isTrue)
{
	for (auto c : backgroundImages)
	{
		if (c->getName() == name)
		{
			c->tickShader(isTrue);
			return;
		}
	}
}

void ScriptLine::tickCharacterShader(const string& name, bool isTrue)
{
	for (auto c : characterImages)
	{
		if (c->getName() == name)
		{
			c->tickShader(isTrue);
			return;
		}
	}
}

void ScriptLine::setCharacterBlurRadius(const string& name, float param)
{
	for (auto c : characterImages)
	{
		if (c->getName() == name)
		{
			c->setShaderParam(param);
			return;
		}
	}
}

void ScriptLine::setBackgroundBlurRadius(const string& name, float param)
{
	for (auto c : backgroundImages)
	{
		if (c->getName() == name)
		{
			c->setShaderParam(param);
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
			c->setPosition(xPos , yPos);
			return;
		}
	}

	// otherwise add a new character
	CharacterImage* c = new CharacterImage(name, expression, xPos, yPos);
	c->setPosition(xPos, yPos);
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
			c->setPosition(xPos, yPos);
			return;
		}
	}

	// otherwise add a new character
	BackgroundImage* c = new BackgroundImage(name, timeOfTheDay, xPos, yPos);
	c->setPosition(xPos , yPos );
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

void ScriptLine::setAllCharacterAlpha(float alpha)
{
	for (auto c : characterImages)
	{
		c->setAlpha(alpha);
	}
	return;
}

void ScriptLine::setAllBackgroundAlpha(float alpha)
{
	for (auto c : backgroundImages)
	{
		c->setAlpha(alpha);
	}
	return;
}

void ScriptLine::setTextboxAlpha(float alpha)
{
	textboxImage->setAlpha(alpha);
}

void ScriptLine::setDialogue(const string& displayname, const string& str)
{
	hideTextbox = false;
	dialogue = UTILITY->addAllNewLines(str, 70);
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
			c->setPosition(xPos, yPos);
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

void ScriptLine::setBgmVolume(float volume, bool relative)
{
	for (auto m : bgm)
	{
		if (m != nullptr)
		{
			float currentFactor = relative ? m->getVolume()/100.f : 1.0f;
			m->setVolume(volume * CONFIG->masterVolume * CONFIG->bgmVolume * 100.f * currentFactor);
		}
	}
}

void ScriptLine::setVoiceVolume(float volume, bool relative)
{
	for (auto m : voices)
	{
		if (m != nullptr)
		{
			float currentFactor = relative ? m->getVolume() / 100.f : 1.0f;
			m->setVolume(volume * CONFIG->masterVolume * CONFIG->voiceVolume * 100.f * currentFactor);
		}
	}
}

void ScriptLine::setSfxVolume(float volume, bool relative)
{
	for (auto m : sfx)
	{
		if (m != nullptr)
		{
			float currentFactor = relative ? m->getVolume() / 100.f : 1.0f;
			m->setVolume(volume * CONFIG->masterVolume * CONFIG->sfxVolume * 100.f * currentFactor);
		}
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

void ScriptLine::stopBgm()
{
	for (auto m : bgm)
	{
		if (m != nullptr) m->stop();
		delete m;
	}
	bgm.clear();
}

void ScriptLine::stopSfx()
{
	for (auto m : sfx)
	{
		if (m != nullptr) m->stop();
		delete m;
	}
	bgm.clear();
}

void ScriptLine::stopVoice()
{
	for (auto m : voices)
	{
		if (m != nullptr) m->stop();
		delete m;
	}
	voices.clear();
}

void ScriptLine::pauseBgm()
{
	for (auto m : bgm)
	{
		if (m != nullptr) m->pause();
	}
}

void ScriptLine::pauseSfx()
{
	for (auto m : sfx)
	{
		if (m != nullptr) m->pause();
	}
}

void ScriptLine::pauseVoice()
{
	for (auto m : voices)
	{
		if (m != nullptr) m->pause();
	}
}

void ScriptLine::resumeBgm()
{
	for (auto m : bgm)
	{
		if (m != nullptr) m->play();
	}
}

void ScriptLine::resumeSfx()
{
	for (auto m : sfx)
	{
		if (m != nullptr) m->play();
	}
}

void ScriptLine::resumeVoice()
{
	for (auto m : voices)
	{
		if (m != nullptr) m->play();
	}
}

void ScriptLine::readNewFile(std::string p_filename)
{
	if (filename != p_filename)
	{
		// open a new file
		filename = p_filename;
		file.close();
		file.open(filename);
	}
	else
	{
		file.seekg(file.beg);
	}
}

void ScriptLine::readNewFileToAnchor(std::string filename, std::string anchor)
{
	readNewFile(filename);

	bool stop = false;
	while (!stop)
	{
		if (file.eof())
		{
			LOGGER->Log("ScriptLine", "EOF of file reached unexpectedly!");
			break;
		}

		std::string line;
		std::getline(file, line);
		line = UTILITY->cutLine(line, "#");	// use # for comments

		std::vector<std::string> tokens = UTILITY->split(line, '|');
		tokens = UTILITY->trim(tokens);

		if (tokens.size() > 0)
		{
			std::string cmdWord = UTILITY->toLower(tokens[0]);
			if (cmdWord == "anchor")
			{
				if (tokens[3] == anchor) 
				{
					stop = true;
				}
			}
		}
	}
}
