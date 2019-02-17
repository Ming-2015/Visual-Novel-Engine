#include "ScriptManager.h"

ScriptManager::ScriptManager(std::string filename) :
	filename(filename)
{
	//init();
}

ScriptManager::~ScriptManager()
{
	if (currentScriptLine != nullptr)
		delete currentScriptLine;
	if (file.is_open()) file.close();

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

std::string ScriptManager::getBGMFileName() const
{
	return currentScriptLine->BGMFileName;
}

int ScriptManager::getNumChoices() const
{
	return currentScriptLine->numChoices;
}

bool ScriptManager::isChoice() const
{
	return currentScriptLine->isChoice;
}

std::vector<std::string> ScriptManager::getChoices() const
{
	return currentScriptLine->choices;
}

std::vector<std::string> ScriptManager::getNextFileNames() const
{
	return currentScriptLine->nextFileNames;
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
	file.open(filename);

	if (!file)
	{
		string err = "Cannot open script: " + filename;
		LOGGER->Log("ScriptManager", err);
		return;
	}

	readCommands();
}

void ScriptManager::update(float delta_t)
{
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
}

void ScriptManager::handleInput(sf::Event & e, sf::RenderWindow & window)
{
	switch (e.type)
	{
		case sf::Event::MouseButtonPressed:
		{
			for (auto c : commands)
			{
				c->skipUpdate();
			}
			break;
		}
	}
}

bool ScriptManager::eof()
{
	return file.eof();
}

bool ScriptManager::doneAllCommands()
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
	if (file && !file.eof())
	{
		bool stop = false;
		while (!stop)
		{
			if (file.eof())
			{
				LOGGER->Log("ScriptManager", "EOF of file reached unexpectedly!");
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
				ScriptCommand* command = nullptr;

				if (cmdWord == "show")
				{
					command = new ShowCommand(tokens);
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
				else if (cmdWord == "jump" && tokens.size() >= 4)
				{
					readNewFile(GLOBAL->ResourceRoot + tokens[3] + ".csv");
				}
				else
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
	if (this->filename != filename)
	{
		// open a new file
		this->filename = filename;
		file.close();
		file.open(filename);
	}
	else
	{
		file.seekg(file.beg);
	}
}