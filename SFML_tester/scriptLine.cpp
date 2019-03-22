#include "scriptLine.h"
using namespace std;

ScriptLine::ScriptLine() 
{
	linelog = new LineLog();
	textboxImage = new TextboxImage();
	textboxImage->setTextboxAlpha(CONFIG->textWindowAlpha * 255.f);
	cinematicBars = new CinematicBars();
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

	if (linelog) delete linelog;
	if (cinematicBars) delete cinematicBars;
}

ScriptLine::ScriptLine(ifstream & savefile)
{
	int pos;

	try {
		dialogue = UTILITY->readFromBinaryFile(savefile);
		name = UTILITY->readFromBinaryFile(savefile);
		textboxImage = new TextboxImage(savefile);
		savefile.read(reinterpret_cast<char *>(&hideTextbox), sizeof(hideTextbox));

		fn_bgm = UTILITY->readVectorFromBinaryFile(savefile);
		fn_voices = UTILITY->readVectorFromBinaryFile(savefile);
		fn_sfx = UTILITY->readVectorFromBinaryFile(savefile);

		savefile.read(reinterpret_cast<char *>(&isChoice), sizeof(isChoice));

		int size;
		savefile.read(reinterpret_cast<char *>(&size), sizeof(size));
		for (int i = 0; i < size; i++)
		{
			choiceImages.push_back(new ChoiceImage(savefile));
		}

		savefile.read(reinterpret_cast<char *>(&size), sizeof(size));
		for (int i = 0; i < size; i++)
		{
			characterImages.push_back(new CharacterImage(savefile));
		}

		savefile.read(reinterpret_cast<char *>(&size), sizeof(size));
		for (int i = 0; i < size; i++)
		{
			backgroundImages.push_back(new BackgroundImage(savefile));
		}

		filename = UTILITY->readFromBinaryFile(savefile);
		savefile.read(reinterpret_cast<char *>(&pos), sizeof(pos));

		playerName = UTILITY->readFromBinaryFile(savefile);

		savefile.read(reinterpret_cast<char *>(&size), sizeof(size));
		for (int i = 0; i < size; i++)
		{
			userFlags.insert( UTILITY->readFromBinaryFile(savefile) );
		}

		cinematicBars = new CinematicBars(savefile);
	}
	catch (exception e)
	{
		LOGGER->Log("ScriptLine", "Unable to read the object from save data!");
		return;
	}

	// open the script file
	file.open(filename);
	file.seekg(pos);

	// play the sounds that were being played then
	for (string fn : fn_bgm)
	{
		sf::Music* music = new sf::Music();
		if (!music->openFromFile(fn))
		{
			string err = "Unable to load bgm file: " + fn;
			LOGGER->Log("ScriptLine", err);
			delete music;
			return;
		}

		bgm.push_back(music);
		music->setLoop(true);
		music->setVolume(1.f * CONFIG->bgmVolume * CONFIG->masterVolume * 100.f);
		music->play();
	}

	for (string fn : fn_voices)
	{
		sf::Music* music = new sf::Music();
		if (!music->openFromFile(fn))
		{
			string err = "Unable to load voice file: " + fn;
			LOGGER->Log("ScriptLine", err);
			delete music;
			return;
		}

		voices.push_back(music);
		music->setLoop(false);
		music->setVolume(1.f * CONFIG->voiceVolume * CONFIG->masterVolume * 100.f);
		music->play();
	}

	for (string fn : fn_sfx)
	{
		sf::Music* music = new sf::Music();
		if (!music->openFromFile(fn))
		{
			string err = "Unable to load sfx file: " + fn;
			LOGGER->Log("ScriptLine", err);
			delete music;
			return;
		}

		sfx.push_back(music);
		music->setLoop(false);
		music->setVolume(1.f * CONFIG->sfxVolume * CONFIG->masterVolume * 100.f);
		music->play();
	}

	linelog = new LineLog();
}

void ScriptLine::serialize(ofstream & savefile)
{
	UTILITY->writeToBinaryFile(savefile, dialogue);
	UTILITY->writeToBinaryFile(savefile, name);
	textboxImage->serialize(savefile);
	savefile.write(reinterpret_cast<const char *>(&hideTextbox), sizeof(hideTextbox));

	UTILITY->writeVectorToBinaryFile(savefile, fn_bgm);
	UTILITY->writeVectorToBinaryFile(savefile, fn_voices);
	UTILITY->writeVectorToBinaryFile(savefile, fn_sfx);

	savefile.write(reinterpret_cast<const char *>(&isChoice), sizeof(isChoice));
	
	int size;
	size = choiceImages.size();
	savefile.write(reinterpret_cast<const char *>(&size), sizeof(size));
	for (int i = 0; i < size; i++)
	{
		choiceImages[i]->serialize(savefile);
	}

	size = characterImages.size();
	savefile.write(reinterpret_cast<const char *>(&size), sizeof(size));
	for (int i = 0; i < size; i++)
	{
		characterImages[i]->serialize(savefile);
	}

	size = backgroundImages.size();
	savefile.write(reinterpret_cast<const char *>(&size), sizeof(size));
	for (int i = 0; i < size; i++)
	{
		backgroundImages[i]->serialize(savefile);
	}

	UTILITY->writeToBinaryFile(savefile, filename);
	int pos = file.tellg();
	savefile.write(reinterpret_cast<const char *>(&pos), sizeof(pos));

	UTILITY->writeToBinaryFile(savefile, playerName);

	size = userFlags.size();
	savefile.write(reinterpret_cast<const char *>(&size), sizeof(size));
	for (auto it = userFlags.begin(); it != userFlags.end(); it++)
	{
		UTILITY->writeToBinaryFile(savefile, *it);
	}

	cinematicBars->serialize(savefile);
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
			c->setFragShader(shaderSrc);
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
			c->setFragShader(shaderSrc);
			return;
		}
	}
}

float ScriptLine::getCharacterBeginBlurR(const string& name)
{
	for (auto c : characterImages)
	{
		if (c->getName() == name)
		{
			return c->getShaderParam("blur_radius");
		}
	}
}

float ScriptLine::getBackgroundBeginBlurR(const string& name)
{
	{
		for (auto c : backgroundImages)
		{
			if (c->getName() == name)
			{
				return c->getShaderParam("blur_radius");
			}
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
			c->setShaderParam("blur_radius", param);
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
			c->setShaderParam("blur_radius", param);
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
	textboxImage->setFontAlpha(alpha);
	textboxImage->setTextboxAlpha(alpha*CONFIG->textWindowAlpha);
}

void ScriptLine::setDialogue(const string& displayname, const string& str)
{
	hideTextbox = false;
	dialogue = UTILITY->addAllNewLines(str, 70);
	textboxImage->setTextboxColor(GLOBAL->getTextboxColor(displayname));

	name = displayname;
	if (UTILITY->toLower(name) == "player")
	{
		name = playerName;
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

void ScriptLine::setBgm(const string & groupname, const string & filename, bool clearOthers, bool repeat, float volume)
{
	if (clearOthers)
	{
		for (int i = 0; i < bgm.size(); i++)
		{
			bgm[i]->stop();
			delete bgm[i];
		}
		bgm.clear();
		fn_bgm.clear();
	}

	string f = GLOBAL->SoundRoot + groupname + "/" + filename;
	sf::Music* music = new sf::Music();
	if (!music->openFromFile(f))
	{
		string err = "Unable to load bgm file: " + f;
		LOGGER->Log("ScriptLine", err);
		delete music;
		return;
	}

	bgm.push_back(music);
	fn_bgm.push_back(f);
	music->setLoop(repeat);
	music->setVolume(volume * CONFIG->bgmVolume * CONFIG->masterVolume * 100.f);
	music->play();
}

void ScriptLine::setVoice(const string & groupname, const string & filename, bool clearOthers, bool repeat, float volume)
{
	if (clearOthers)
	{
		for (int i = 0; i < voices.size(); i++)
		{
			voices[i]->stop();
			delete voices[i];
		}
		voices.clear();
		fn_voices.clear();
	}

	string f = GLOBAL->SoundRoot + groupname + "/" + filename;
	sf::Music* music = new sf::Music();
	if (!music->openFromFile(f))
	{
		string err = "Unable to load music file: " + f;
		LOGGER->Log("ScriptLine", err);
		delete music;
		return;
	}

	voices.push_back(music);
	fn_voices.push_back(f);
	music->setLoop(repeat);
	music->setVolume(volume * CONFIG->voiceVolume * CONFIG->masterVolume * 100.f);
	music->play();
}

void ScriptLine::setSfx(const string & groupname, const string & filename, bool clearOthers, bool repeat, float volume)
{
	if (clearOthers)
	{
		for (int i = 0; i < sfx.size(); i++)
		{
			sfx[i]->stop();
			delete sfx[i];
		}
		sfx.clear();
		fn_sfx.clear();
	}

	string f = GLOBAL->SoundRoot + groupname + "/" + filename;
	sf::Music* music = new sf::Music();
	if (!music->openFromFile(f))
	{
		string err = "Unable to load music file: " + f;
		LOGGER->Log("ScriptLine", err);
		delete music;
		return;
	}

	sfx.push_back(music);
	fn_sfx.push_back(f);
	music->setLoop(repeat);
	music->setVolume(volume * CONFIG->sfxVolume * CONFIG->masterVolume * 100.f);
	music->play();
	return;
}

void ScriptLine::setBgmVolume(float volume, bool relative)
{
	for (auto m : bgm)
	{
		if (m != nullptr)
		{
			float currentFactor = relative ? m->getVolume() / 100.f / (CONFIG->masterVolume * CONFIG->bgmVolume) : 1.0f;
			m->setVolume(volume * currentFactor * CONFIG->masterVolume * CONFIG->bgmVolume * 100.f);
		}
	}
}

void ScriptLine::setVoiceVolume(float volume, bool relative)
{
	for (auto m : voices)
	{
		if (m != nullptr)
		{
			float currentFactor = relative ? m->getVolume() / 100.f / (CONFIG->masterVolume * CONFIG->voiceVolume) : 1.0f;
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
			float currentFactor = relative ? m->getVolume() / 100.f / (CONFIG->masterVolume * CONFIG->sfxVolume) : 1.0f;
			m->setVolume(volume * CONFIG->masterVolume * CONFIG->sfxVolume * 100.f * currentFactor);
		}
	}
}

void ScriptLine::setBgmVolume(float volume, string folder, string name)
{
	string fname = GLOBAL->SoundRoot + folder + "/" + name;
	for (int i = 0; i < fn_bgm.size(); i++)
	{
		if (fn_bgm[i] == fname)
		{
			sf::Music*& m = bgm[i];
			if (m != nullptr) m->setVolume(volume * CONFIG->masterVolume * CONFIG->bgmVolume * 100.f);
		}
	}
}

void ScriptLine::setVoiceVolume(float volume, string folder, string name)
{
	string fname = GLOBAL->SoundRoot + folder + "/" + name;
	for (int i = 0; i < fn_voices.size(); i++)
	{
		if (fn_voices[i] == fname)
		{
			sf::Music*& m = voices[i];
			if (m != nullptr) m->setVolume(volume * CONFIG->masterVolume * CONFIG->voiceVolume * 100.f);
		}
	}
}

void ScriptLine::setSfxVolume(float volume, string folder, string name)
{
	string fname = GLOBAL->SoundRoot + folder + "/" + name;
	for (int i = 0; i < fn_sfx.size(); i++)
	{
		if (fn_sfx[i] == fname)
		{
			sf::Music*& m = sfx[i];
			if (m != nullptr) m->setVolume(volume * CONFIG->masterVolume * CONFIG->sfxVolume * 100.f);
		}
	}
}

void ScriptLine::stopBgm()
{
	for (auto m : bgm)
	{
		if (m != nullptr) m->stop();
		delete m;
	}
	bgm.clear();
	fn_bgm.clear();
}

void ScriptLine::stopSfx()
{
	for (auto m : sfx)
	{
		if (m != nullptr) m->stop();
		delete m;
	}
	sfx.clear();
	fn_sfx.clear();
}

void ScriptLine::stopVoice()
{
	for (auto m : voices)
	{
		if (m != nullptr) m->pause();
		delete m;
	}
	voices.clear();
	fn_voices.clear();
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

void ScriptLine::updateSoundList()
{
	//for (int i = 0; i < bgm.size(); i++)
	//{
	//	if (bgm[i]->getStatus() == sf::Music::Stopped)
	//	{
	//		delete bgm[i];
	//		bgm.erase(bgm.begin() + i);
	//		fn_bgm.erase(fn_bgm.begin() + i);
	//		i--;
	//	}
	//}

	for (int i = 0; i < sfx.size(); i++)
	{
		if (sfx[i]->getStatus() == sf::Music::Stopped)
		{
			delete sfx[i];
			sfx.erase(sfx.begin() + i);
			fn_sfx.erase(fn_sfx.begin() + i);
			i--;
		}
	}

	//for (int i = 0; i < voices.size(); i++)
	//{
	//	if (voices[i]->getStatus() == sf::Music::Stopped)
	//	{
	//		delete voices[i];
	//		voices.erase(voices.begin() + i);
	//		fn_voices.erase(fn_voices.begin() + i);
	//		i--;
	//	}
	//}
}

bool ScriptLine::isVoicePlayed() const
{
	bool done = true;
	for (sf::Music* m : voices)
	{
		if (m->getStatus() != sf::Music::Stopped)
		{
			done = false;
			break;
		}
	}
	return done;
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

std::string ScriptLine::getPrevBgmFileName() const
{
	return fn_bgm.size() <= 0 ? "" : fn_bgm[fn_bgm.size() - 1];
}

std::string ScriptLine::getPrevVoiceFilename() const
{
	return fn_voices.size() <= 0 ? "" : fn_voices[fn_voices.size() - 1];
}

void ScriptLine::removeLoop(std::string loopName)
{
	loopsToRemove.push_back(loopName);
}

void ScriptLine::appendLineToLog(std::string name, std::string line, std::vector<std::string> voicefiles)
{
	if (UTILITY->toLower(name) == "player")
	{
		name = GLOBAL->playerName;
	}

	LineLogItem logItem;
	logItem.name = name;
	logItem.line = line;
	logItem.flags = userFlags;
	logItem.scriptFile = filename;
	logItem.scriptFilePos = file.tellg();
	logItem.voiceFile = voicefiles;
	logItem.musicFile = getPrevBgmFileName();
	linelog->addLogItem(logItem);
}

void ScriptLine::setCinematicBarsHeight(float height)
{
	cinematicBars->setHeight(height);
}

float ScriptLine::getCinematicBarsHeight() const
{
	return cinematicBars->getHeight();
}

void ScriptLine::displayCinematicBars(bool display)
{
	cinematicBars->setDisplay(display);
}

bool ScriptLine::isDisplayingCinematicBars() const
{
	return cinematicBars->isDisplaying();
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
