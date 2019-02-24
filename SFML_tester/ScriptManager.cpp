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
		int prevPos = file.tellg();
		file.read(reinterpret_cast<char*> (&commandType), sizeof(commandType));
		file.seekg(prevPos);
		switch (commandType)
		{
			case ScriptCommand::COMMAND_BLUR:
			{
				commands.push_back(new BlurCommand(file));
				break;
			}
			case ScriptCommand::COMMAND_CLEAR:
			{
				commands.push_back(new ClearCommand(file));
				break;
			}
			case ScriptCommand::COMMAND_DELAY:
			{
				commands.push_back(new DelayCommand(file));
				break;
			}
			case ScriptCommand::COMMAND_DISPLAY:
			{
				commands.push_back(new DisplayCommand(file));
				break;
			}
			case ScriptCommand::COMMAND_FLASH:
			{
				commands.push_back(new FlashCommand(file));
				break;
			}
			case ScriptCommand::COMMAND_HIDE:
			{
				commands.push_back(new HideCommand(file));
				break;
			}
			case ScriptCommand::COMMAND_JUMP:
			{
				commands.push_back(new JumpCommand(file));
				break;
			}
			case ScriptCommand::COMMAND_MOVE:
			{
				commands.push_back(new MoveCommand(file));
				break;
			}
			case ScriptCommand::COMMAND_PAUSE:
			{
				commands.push_back(new PauseCommand(file));
				break;
			}
			case ScriptCommand::COMMAND_PLAY:
			{
				commands.push_back(new PlayCommand(file));
				break;
			}
			case ScriptCommand::COMMAND_REMOVE:
			{
				commands.push_back(new RemoveCommand(file));
				break;
			}
			case ScriptCommand::COMMAND_RESUME:
			{
				commands.push_back(new ResumeCommand(file));
				break;
			}
			case ScriptCommand::COMMAND_ROTATE:
			{
				commands.push_back(new RotateCommand(file));
				break;
			}
			case ScriptCommand::COMMAND_SET:
			{
				commands.push_back(new SetCommand(file));
				break;
			}
			case ScriptCommand::COMMAND_SHOW:
			{
				commands.push_back(new ShowCommand(file));
				break;
			}
			case ScriptCommand::COMMAND_STOP:
			{
				commands.push_back(new StopCommand(file));
				break;
			}
			case ScriptCommand::COMMAND_UNHIDE:
			{
				commands.push_back(new UnhideCommand(file));
				break;
			}
			case ScriptCommand::COMMAND_ZOOM:
			{
				commands.push_back(new ZoomCommand(file));
				break;
			}
			default:
			{
				std::string err = "Found an invalid command type code: " + commandType;
				LOGGER->Log("ScriptManager", err);
				break;
			}
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
	}

	for (auto it = commands.begin(); it != commands.end();)
	{
		bool incrementIt = true;
		
		if (*it != nullptr)
		{
			(*it)->update(delta_t);
			(*it)->execute(currentScriptLine);

			if ((*it)->isDone())
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
}

void ScriptManager::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

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
				std::string cmdWord = UTILITY->toLower(tokens[0]);
				ScriptCommand* command = nullptr;

				if (cmdWord == "show")
				{
					command = new ShowCommand(tokens);
				}
				else if (cmdWord == "remove")
				{
					command = new RemoveCommand(tokens);
				}
				else if (cmdWord == "clear")
				{
					command = new ClearCommand(tokens);
				}
				else if (cmdWord == "flash")
				{
					command = new FlashCommand(tokens);
				}
				else if (cmdWord == "blur")
				{
					command = new BlurCommand(tokens);
				}
				else if (cmdWord == "display")
				{
					command = new DisplayCommand(tokens);
				}
				else if (cmdWord == "set")
				{
					command = new SetCommand(tokens);
				}
				else if (cmdWord == "move")
				{
					command = new MoveCommand(tokens);
				}
				else if (cmdWord == "rotate")
				{
					command = new RotateCommand(tokens);
				}
				else if (cmdWord == "zoom")
				{
					command = new ZoomCommand(tokens);
				}
				else if (cmdWord == "play")
				{
					command = new PlayCommand(tokens);
				}
				else if (cmdWord == "stop")
				{
					command = new StopCommand(tokens);
				}
				else if (cmdWord == "pause")
				{
					command = new PauseCommand(tokens);
				}
				else if (cmdWord == "resume")
				{
					command = new ResumeCommand(tokens);
				}
				else if (cmdWord == "hide")
				{
					command = new HideCommand(tokens);
				}
				else if (cmdWord == "delay")
				{
					command = new DelayCommand(tokens);
				}
				else if (cmdWord == "unhide")
				{
					command = new UnhideCommand(tokens);
				}
				else if (cmdWord == "jump" && tokens.size() >= 4)
				{
					command = new JumpCommand(tokens);
				}
				else if (cmdWord != "")
				{
					string msg = "Invalid Command found: " + cmdWord;
					LOGGER->Log("ScriptManager", msg);
					command = nullptr;
				}

				if (command != nullptr)
				{
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
			}
		}
	}
	else
	{
		LOGGER->Log("ScriptManager", "Reached EOF of current Script File!");
	}
}
