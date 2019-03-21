#include "ScriptManager.h"

ScriptManager::ScriptManager(std::string filename) :
	initFileName(filename)
{
	init();
}

ScriptManager::~ScriptManager()
{
	if (currentScriptLine->file.is_open()) 
		currentScriptLine->file.close();

	if (currentScriptLine != nullptr)
		delete currentScriptLine;

	for (auto c : commands)
	{
		if (c != nullptr) delete c;
	}
}

ScriptManager::ScriptManager(ifstream & file)
{
	int size;
	file.read(reinterpret_cast<char*> (&size), sizeof(size));
	for (int i = 0; i < size; i++)
	{
		int commandType;
		ScriptCommand* command = nullptr;		
		ScriptCommandFactory::GenerateCommandByFile(file, command, commandType);

		if (command != nullptr)
		{
			commands.push_back(command);
		}
	}

	currentScriptLine = new ScriptLine(file);

	initFileName = UTILITY->readFromBinaryFile(file);
}

void ScriptManager::serialize(ofstream & savefile) const
{
	int size = commands.size();
	savefile.write(reinterpret_cast<const char*> (&size), sizeof(size));
	for (int i = 0; i < size; i++)
	{
		commands[i]->serialize(savefile);
	}

	currentScriptLine->serialize(savefile);

	UTILITY->writeToBinaryFile(savefile, initFileName);
}

std::string ScriptManager::getScriptLine() const
{
	return currentScriptLine->dialogue;
}

std::string ScriptManager::getDisplayName() const 
{
	return currentScriptLine->name;
}

const TextboxImage * ScriptManager::getTextboxImage() const
{
	return currentScriptLine->textboxImage;
}

bool ScriptManager::shouldHideTextbox() const
{
	return currentScriptLine->hideTextbox || shouldCloseTextbox;
}

std::vector<ChoiceImage*> ScriptManager::getChoices() const
{
	return currentScriptLine->choiceImages;
}

bool ScriptManager::isChoice() const
{
	return currentScriptLine->isChoice;
}

std::vector<CharacterImage* > ScriptManager::getCharacterImages() const
{
	return currentScriptLine->characterImages;
}

std::vector<BackgroundImage*> ScriptManager::getBackgroundImages() const
{
	return currentScriptLine->backgroundImages;
}

std::string ScriptManager::getCurrentFileName() const
{
	return std::string();
}

void ScriptManager::init()
{
	currentScriptLine = new ScriptLine();
	currentScriptLine->file.open(initFileName);
	currentScriptLine->filename = initFileName;

	if (!currentScriptLine->file)
	{
		string err = "Cannot open script: " + initFileName;
		LOGGER->Log("ScriptManager", err);
		return;
	}

	readCommands();
}

void ScriptManager::update(float delta_t)
{
	if (doneAllCommands())
	{
		readCommands();
	}

	if (GLOBAL->skipMode)
	{
		for (auto c : commands)
		{
			c->skipUpdate();
		}
	}

	else if ( (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) 
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) && GLOBAL->windowPtr->hasFocus() )
	{
		for (auto c : commands)
		{
			c->skipUpdate();
		}
		GLOBAL->ctrlSkipMode = true;
	}
	else
	{
		GLOBAL->ctrlSkipMode = false;
	}


	for (auto it = commands.begin(); it != commands.end();)
	{
		bool incrementIt = true;
		
		if (*it != nullptr)
		{
			(*it)->update(delta_t);
			(*it)->execute(currentScriptLine);

			if ((*it)->isDone() || !(*it)->validArgs() )
			{
				delete *it;
				it = commands.erase(it);
				incrementIt = false;
			}
		}

		if (incrementIt) it++;
	}

	for (auto c : currentScriptLine->backgroundImages)
	{
		if (c != nullptr) c->update(delta_t);
	}

	for (auto c : currentScriptLine->characterImages)
	{
		if (c != nullptr) c->update(delta_t);
	}

	if (currentScriptLine->textboxImage != nullptr)
	{
		currentScriptLine->textboxImage->update(delta_t);
	}

	currentScriptLine->updateSoundList();

	// check the volumes - NOTE THAT WE ARE UPDATING ONE SCRIPTMANAGER AT A TIME
	if (s_masterVolume != CONFIG->masterVolume)
	{
		s_masterVolume = CONFIG->masterVolume;
		currentScriptLine->setBgmVolume(1.0, false);
		currentScriptLine->setSfxVolume(1.0, false);
		currentScriptLine->setVoiceVolume(1.0, false);
	}
	if (s_bgmVolume != CONFIG->bgmVolume)
	{
		s_bgmVolume = CONFIG->bgmVolume;
		currentScriptLine->setBgmVolume(1.0, false);
	}
	if (s_sfxVolume != CONFIG->sfxVolume)
	{
		s_sfxVolume = CONFIG->sfxVolume;
		currentScriptLine->setSfxVolume(1.0, false);
	}
	if (s_voiceVolume != CONFIG->voiceVolume)
	{
		s_voiceVolume = CONFIG->voiceVolume;
		currentScriptLine->setVoiceVolume(1.0, false);
	}
	if (s_textWindowAlpha != CONFIG->textWindowAlpha)
	{
		s_textWindowAlpha = CONFIG->textWindowAlpha;
		currentScriptLine->textboxImage->setTextboxAlpha(s_textWindowAlpha * 255.f);
	}
}

void ScriptManager::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	sf::Vector2f mousePosF = CONFIG->getCursorPosition(window);

	switch (e.type)
	{
		case sf::Event::KeyReleased:
		{
			if (e.key.code == sf::Keyboard::Enter && !currentScriptLine->isChoice)
			{
				for (auto c : commands)
				{
					c->skipUpdate();
				}
			}
			break;
		}

		case sf::Event::MouseWheelScrolled:
		{
			if (e.mouseWheelScroll.delta < 0)
			{
				for (auto c : commands)
				{
					c->skipUpdate();
				}
			}
			else if (e.mouseWheelScroll.delta > 0)
			{
				// show backlog
			}
			break;
		}

		case sf::Event::MouseButtonReleased:
		{
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				// if user needs to select a choice
				if (currentScriptLine->isChoice)
				{
					for (auto c : getChoices())
					{
						// if the choice is selected... clear the command and insert new flag
						if (c->getGlobalBoundary().contains(mousePosF))
						{
							for (string flag : c->getFlags())
							{
								currentScriptLine->userFlags.insert(flag);
							}

							currentScriptLine->clearChoices();

							for (auto c : commands)
							{
								c->execute(currentScriptLine);
								c->skipUpdate();
							}

							break;
						}
					}
				}
				
				// otherwise skip the command update
				else
				{
					for (auto c : commands)
					{
						c->skipUpdate();
					}
				}
			}
			break;
		}

		case sf::Event::MouseMoved:
		{
			// if user needs to select a choice
			if (currentScriptLine->isChoice)
			{
				for (auto c : getChoices())
				{
					// change color based on when the choicebox is selected
					if (c->getGlobalBoundary().contains(mousePosF))
					{
						c->setChoiceboxColor( GLOBAL->choiceboxColorSelected );
					}
					else
					{
						c->setChoiceboxColor( GLOBAL->choiceboxColorUnselected );
					}
				}
			}

			break;
		}
	}
}

void ScriptManager::setPlayerName(const std::string & name)
{
	currentScriptLine->playerName = name;
}

std::string ScriptManager::getPlayerName() const
{
	return currentScriptLine->playerName;
}

void ScriptManager::hideTextbox()
{
	shouldCloseTextbox = true;
}

void ScriptManager::showTextbox()
{
	shouldCloseTextbox = false;
}

bool ScriptManager::isTextboxClosed()
{
	return shouldCloseTextbox;
}

LineLog * ScriptManager::getLineLog() const
{
	return currentScriptLine->linelog;
}

void ScriptManager::advanceText()
{

}

bool ScriptManager::eof() const
{
	return currentScriptLine->file.eof();
}

bool ScriptManager::doneAllCommands() const
{
	bool done = true;
	for (auto c : commands)
	{
		if (c->shouldWait()) done = false;
	}
	return done;
}

void ScriptManager::readCommands()
{
	if (currentScriptLine->file && !currentScriptLine->file.eof())
	{
		bool stop = false;
		while (!stop)
		{
			if (currentScriptLine->file.eof())
			{
				LOGGER->Log("ScriptManager", "EOF of file reached unexpectedly!");
				break;
			}

			std::string line;
			std::getline(currentScriptLine->file, line);
			line = UTILITY->cutLine(line, "#");	// use # for comments

			std::vector<std::string> tokens = UTILITY->split(line, '|');
			tokens = UTILITY->trim(tokens);
			if (tokens.size() > 0)
			{
				ScriptCommand* command = nullptr;
				int commandType;

				ScriptCommandFactory::GenerateCommandByTokens(tokens, command, commandType);

				if (commandType == ScriptCommand::COMMAND_ENDLOOP)
				{
					LOGGER->Log("ScriptManager", "Invalid Endloop command: Not inside a loop");
				}
				else if (commandType == ScriptCommand::COMMAND_BREAKLOOP)
				{
					LOGGER->Log("ScriptManager", "Invalid BreakLoop command: not inside a loop");
				}
				else if (commandType == ScriptCommand::COMMAND_CONTINUELOOP)
				{
					LOGGER->Log("ScriptManager", "Invalid ContinueLoop command: not inside a loop");
				}
				else if (commandType == ScriptCommand::COMMAND_CLEARLOOP)
				{
					LOGGER->Log("ScriptManager", "Invalid ClearLoop command: not inside a loop");
				}

				// check if the scriptreader should stop reading
				if (command != nullptr)
				{
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
			}
		}

		// remove the loops, if necessary
		for (int i = 0; i < currentScriptLine->loopsToRemove.size(); i++)
		{
			auto it = StartLoopCommand::loopMap.find(currentScriptLine->loopsToRemove[i]);
			if (it == StartLoopCommand::loopMap.end())
			{
				std::string err = "Unable to find Loop with name: " + currentScriptLine->loopsToRemove[i];
				LOGGER->Log("ScriptManager", err);
			}
			else
			{
				(*it).second->stopLoop();
			}
		}
		currentScriptLine->loopsToRemove.clear();
	}
	else
	{
		LOGGER->Log("ScriptManager", "Reached EOF of current Script File!");
	}

}
