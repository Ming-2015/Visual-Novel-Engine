#include "ScriptManager.h"

ScriptManager::ScriptManager(std::string filename) :
	initFileName(filename)
{
	//init();
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
	return currentScriptLine->hideTextbox;
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

	for (auto it = commands.begin(); it != commands.end();)
	{
		bool incrementIt = true;
		
		if (*it != nullptr)
		{
			(*it)->update(delta_t);
			(*it)->execute(currentScriptLine);

			if ((*it)->isDone())
			{
				(*it) -> skipUpdate();
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
}

void ScriptManager::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

	switch (e.type)
	{
		case sf::Event::MouseButtonReleased:
		{
			// if user needs to select a choice
			if (currentScriptLine->isChoice)
			{
				for (auto c : getChoices())
				{
					// if the choice is selected... clear the command and insert new flag
					if (c->getGlobalBoundary().contains(mousePosF))
					{
						GLOBAL->userFlags.insert(c->getFlag());
						currentScriptLine->clearChoices();

						for (auto c : commands)
						{
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

		case sf::Event::KeyPressed:
		{
			switch (e.key.code) 
			{
				case sf::Keyboard::LControl:
				{
					GLOBAL->skipMode = true;
					break;
				}
				case sf::Keyboard::RControl:
				{
					GLOBAL->skipMode = true;
					break;
				}
			}
			break;
		}

		case sf::Event::KeyReleased:
		{
			switch (e.key.code)
			{
			case sf::Keyboard::LControl:
			{
				GLOBAL->skipMode = false;
				break;
			}
			case sf::Keyboard::RControl:
			{
				GLOBAL->skipMode = false;
				break;
			}
			}
			break;
		}
	}
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
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
				else if (cmdWord == "remove")
				{
					command = new RemoveCommand(tokens);
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
				else if (cmdWord == "clear")
				{
					command = new ClearCommand(tokens);
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
				if (cmdWord == "flash")
				{
					command = new FlashCommand(tokens);
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
				else if (cmdWord == "display")
				{
					command = new DisplayCommand(tokens);
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
				else if (cmdWord == "set")
				{
					command = new SetCommand(tokens);
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
				else if (cmdWord == "move")
				{
					command = new MoveCommand(tokens);
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
				else if (cmdWord == "rotate")
				{
					command = new RotateCommand(tokens);
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
				else if (cmdWord == "zoom")
				{
					command = new ZoomCommand(tokens);
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
				else if (cmdWord == "play")
				{
					command = new PlayCommand(tokens);
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
				else if (cmdWord == "stop")
				{
					command = new StopCommand(tokens);
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
				else if (cmdWord == "pause")
				{
					command = new PauseCommand(tokens);
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
				else if (cmdWord == "resume")
				{
					command = new ResumeCommand(tokens);
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
				else if (cmdWord == "hide")
				{
					command = new HideCommand(tokens);
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
				else if (cmdWord == "delay")
				{
					command = new DelayCommand(tokens);
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
				else if (cmdWord == "unhide")
				{
					command = new UnhideCommand(tokens);
					if (command->shouldWait()) stop = true;
					commands.push_back(command);
				}
				else if (cmdWord == "jump" && tokens.size() >= 4)
				{
					readNewFile(GLOBAL->ResourceRoot + tokens[3] + ".csv");
				}
				else if (cmdWord != "")
				{
					string msg = "Invalid Command found: " + cmdWord;
					LOGGER->Log("ScriptManager", msg);
				}
			}
		}
	}
	else
	{
		LOGGER->Log("ScriptManager", "Reached EOF of current Script File!");
	}
}

void ScriptManager::readNewFile(std::string filename)
{
	if (currentScriptLine->filename != filename)
	{
		// open a new file
		currentScriptLine->filename = filename;
		currentScriptLine->file.close();
		currentScriptLine->file.open(filename);
	}
	else
	{
		currentScriptLine->file.seekg(currentScriptLine->file.beg);
	}
}