#include "StartLoopCommand.h"

std::map < std::string, StartLoopCommand* > StartLoopCommand::loopMap;

StartLoopCommand::StartLoopCommand(std::vector<std::string> args)
	: ScriptCommand(args)
{
	// checking if the first argument is show
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "startloop")
	{
		LOGGER->Log("StartLoopCommand", "Invalid Command Input");
		valid = false;
		return;
	}
	commandType = COMMAND_STARTLOOP;

	// parse the arguments
	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);	
	flag = UTILITY->toLower(args[COLUMN_FLAG]);				

	// name of this loop
	loopName = args[COLUMN_ARG1];
	if (UTILITY->trim(loopName) == "")
	{
		LOGGER->Log("StartLoopCommand", "Invalid Loop Name! Please specify a loop name");
		valid = false;
		return;
	}
	else if (loopMap.find(loopName) != loopMap.end())
	{
		LOGGER->Log("StartLoopCommand", "Duplicate Loop Name found! Please stop previous loop with same name first");
		valid = false;
		return;
	}
	else
	{
		loopMap.insert(std::make_pair(loopName, this));
	}

	totalNumLoop = 1;	// num loop 1 by default
	if (args.size() > COLUMN_ARG2)
	{
		if (args[COLUMN_ARG2] == "infinite" || args[COLUMN_ARG2] == "inf" || args[COLUMN_ARG2] == "i")
		{
			totalNumLoop = -1;
		}
		else try {
			totalNumLoop = std::stof(args[COLUMN_ARG2]);	// column 6 : x position to display
		}
		catch (exception e)
		{
			LOGGER->Log("StartLoopCommand", "Invalid loop count input");
		}
	}

	// check the flag validity
	wait = true; // need to wait 1 iteration to reach execute
	if (flag == "" || flag == "none" || time == 0)
	{
		flagType = FLAG_NONE;
	}
	else if (flag == "wait" || flag == "w")
	{
		flagType = FLAG_WAIT;
	}
	else
	{
		LOGGER->Log("StartLoopCommand", "Invalid Flag");
		valid = false;
		return;
	}

	// check object type validity
	if (objectTypeName == "loop" || objectTypeName == "l" || objectTypeName == "")
	{
		objectType = OBJECT_LOOP;
	}
	else
	{
		valid = false;
		LOGGER->Log("StartLoopCommand", "Invalid Object Type");
		return;
	}
}

StartLoopCommand::~StartLoopCommand()
{
}

StartLoopCommand::StartLoopCommand(ifstream & savefile)
	: ScriptCommand(savefile)
{

	objectTypeName = UTILITY->readFromBinaryFile(savefile);
	flag = UTILITY->readFromBinaryFile(savefile);
	loopName = UTILITY->readFromBinaryFile(savefile);

	savefile.read(reinterpret_cast<char *> (&flagType), sizeof(flagType));
	savefile.read(reinterpret_cast<char *> (&objectType), sizeof(objectType));

	savefile.read(reinterpret_cast<char *> (&totalNumLoop), sizeof(totalNumLoop));
	savefile.read(reinterpret_cast<char *> (&loopCount), sizeof(loopCount));
	savefile.read(reinterpret_cast<char *> (&commandIdx), sizeof(commandIdx));

	savefile.read(reinterpret_cast<char *> (&initialized), sizeof(initialized));

	int size;
	savefile.read(reinterpret_cast<char *> (&size), sizeof(size));
	for (int i = 0; i < size; i++)
	{
		tokensList.push_back(UTILITY->readVectorFromBinaryFile(savefile));
	}

	savefile.read(reinterpret_cast<char*> (&size), sizeof(size));
	for (int i = 0; i < size; i++)
	{
		int commandType;
		ScriptCommand* command = nullptr;
		ScriptCommandFactory::GenerateCommandByFile(savefile, command, commandType);
		if (command != nullptr)
		{
			executingCommandsList.push_back(command);
		}
	}
	if (valid)
	{
		loopMap.insert(make_pair(loopName, this));
	}
}

void StartLoopCommand::serialize(ofstream & savefile) const
{
	ScriptCommand::serialize(savefile);

	UTILITY->writeToBinaryFile(savefile, objectTypeName);
	UTILITY->writeToBinaryFile(savefile, flag);
	UTILITY->writeToBinaryFile(savefile, loopName);

	savefile.write(reinterpret_cast<const char *> (&flagType), sizeof(flagType));
	savefile.write(reinterpret_cast<const char *> (&objectType), sizeof(objectType));

	savefile.write(reinterpret_cast<const char *> (&totalNumLoop), sizeof(totalNumLoop));
	savefile.write(reinterpret_cast<const char *> (&loopCount), sizeof(loopCount));
	savefile.write(reinterpret_cast<const char *> (&commandIdx), sizeof(commandIdx));

	savefile.write(reinterpret_cast<const char *> (&initialized), sizeof(initialized));

	// write the list of command tokens
	int size = tokensList.size();
	savefile.write(reinterpret_cast<const char *> (&size), sizeof(size));
	for (int i = 0; i < size; i++)
	{
		UTILITY->writeVectorToBinaryFile(savefile, tokensList[i]);
	}

	// write the currently executing commands 
	size = executingCommandsList.size();
	savefile.write(reinterpret_cast<const char*> (&size), sizeof(size));
	for (int i = 0; i < size; i++)
	{
		executingCommandsList[i]->serialize(savefile);
	}
}

void StartLoopCommand::execute(ScriptLine * scriptLine)
{
	if (!valid) return;

	if (!initialized)
	{
		initialized = true;

		// initialize tokens list
		initTokensList(scriptLine, tokensList);

		// initialize the executingCommandsList
		readCommands(scriptLine, executingCommandsList, commandIdx, loopCount, totalNumLoop);
	}

	if (flagType == FLAG_NONE)
	{
		wait = false;
	}
	else if (flagType == FLAG_WAIT)
	{
		wait = true;
	}

	// execute all the commands and delete the ones that are done
	for (auto it = executingCommandsList.begin(); it != executingCommandsList.end();)
	{
		bool incrementIt = true;

		if (*it != nullptr)
		{
			(*it)->execute(scriptLine);

			if ((*it)->isDone() || !(*it)->validArgs())
			{
				delete *it;
				it = executingCommandsList.erase(it);
				incrementIt = false;
			}
		}

		if (incrementIt) it++;
	}

	// read new commands if needed
	if (doneAllCommands())
	{
		readCommands(scriptLine, executingCommandsList, commandIdx, loopCount, totalNumLoop);
	}

	// if everything is executed, stop the loop
	if (executingCommandsList.empty())
	{
		done = true;
		wait = false;

		if (loopMap.find(loopName) != loopMap.end())
		{
			loopMap.erase(loopName);
		}
	}
}

void StartLoopCommand::skipUpdate()
{
	if (!valid) return;

	if (flagType == FLAG_WAIT)
	{
		for (auto c : executingCommandsList)
		{
			c->skipUpdate();
		}
	}
}

void StartLoopCommand::update(float delta_t)
{
	if (!valid) return;

	for (auto c : executingCommandsList)
	{
		c->update(delta_t);
	}
}

void StartLoopCommand::stopLoop()
{
	for (auto c : executingCommandsList)
	{
		delete c;
	}
	executingCommandsList.clear();
	tokensList.clear();
	done = true;
	wait = false;

	if (loopMap.find(loopName) != loopMap.end())
	{
		loopMap.erase(loopName);
	}
}

bool StartLoopCommand::doneAllCommands() const
{
	bool finished = true;
	for (auto c : executingCommandsList)
	{
		if (c->shouldWait()) 
		{
			finished = false;
			break;
		}
	}
	return finished;
}

void StartLoopCommand::readCommands(ScriptLine* currentScriptLine, 
	std::vector<ScriptCommand*>& executingCommandsList,
	int& commandIdx, int& loopCount, const int& maxLoopCount)
{
	// edge cases
	if (commandIdx >= tokensList.size()) commandIdx = 0;
	if (tokensList.size() == 0) return;
	if (maxLoopCount >= 0 && loopCount >= maxLoopCount) return;

	bool shouldStop = false;
	while (!shouldStop)
	{
		std::vector<std::string> tokens = tokensList[commandIdx];
		int commandType;
		ScriptCommand* command = nullptr;
		ScriptCommandFactory::GenerateCommandByTokens(tokens, command, commandType);

		if (commandType == COMMAND_BREAKLOOP)
		{
			std::string flag = UTILITY->toLower(tokens[COLUMN_FLAG]);
			std::string flagStr = tokens.size() > COLUMN_ARG1 ? tokens[COLUMN_ARG1] : "";

			bool shouldExecute;
			if (flag == "flag" || flag == "f")
			{
				shouldExecute = UTILITY->evaluateFlagExpression(currentScriptLine->userFlags, flagStr);
			}
			else if (flag == "none" || flag == "n" || flag == "")
			{
				shouldExecute = true;
			}
			else
			{
				shouldExecute = false;
				LOGGER->Log("StartLoopCommand", "Invalid argument for BreakLoop command!");
			}

			if (shouldExecute)
			{
				stopLoop();
				shouldStop = true;
			}
		}
		else if (commandType == COMMAND_CONTINUELOOP)
		{
			command = nullptr;
			commandType = ScriptCommand::COMMAND_CONTINUELOOP;

			std::string flag = UTILITY->toLower(tokens[COLUMN_FLAG]);
			std::string flagStr = tokens.size() > COLUMN_ARG1 ? tokens[COLUMN_ARG1] : "";

			bool shouldExecute;
			if (flag == "flag" || flag == "f")
			{
				shouldExecute = UTILITY->evaluateFlagExpression(currentScriptLine->userFlags, flagStr);
			}
			else if (flag == "none" || flag == "n" || flag == "")
			{
				shouldExecute = true;
			}
			else
			{
				shouldExecute = false;
				LOGGER->Log("StartLoopCommand", "Invalid argument for ContinueLoop command!");
			}

			if (shouldExecute)
			{
				// jump to the last index
				commandIdx = tokensList.size() - 1;
			}
		}
		else if (commandType == COMMAND_CLEARLOOP)
		{
			command = nullptr;
			commandType = ScriptCommand::COMMAND_CLEARLOOP;

			std::string flag = UTILITY->toLower(tokens[COLUMN_FLAG]);
			std::string flagStr = tokens.size() > COLUMN_ARG1 ? tokens[COLUMN_ARG1] : "";

			bool shouldExecute;
			if (flag == "flag" || flag == "f")
			{
				shouldExecute = UTILITY->evaluateFlagExpression(currentScriptLine->userFlags, flagStr);
			}
			else if (flag == "none" || flag == "n" || flag == "")
			{
				shouldExecute = true;
			}
			else
			{
				shouldExecute = false;
				LOGGER->Log("StartLoopCommand", "Invalid argument for ContinueLoop command!");
			}

			if (shouldExecute)
			{
				// jump to the last index
				for (auto c : executingCommandsList)
				{
					delete c;
				}
				executingCommandsList.clear();
			}
		}

		// check if the scriptreader should stop reading
		if (command != nullptr)
		{
			executingCommandsList.push_back(command);
			if (command->shouldWait()) shouldStop = true;
		}

		// increment commandIdx, or loop back if necessary
		if (++commandIdx >= tokensList.size())
		{
			commandIdx = 0;
			// note that negative loop count means infinite
			if (++loopCount >= maxLoopCount && maxLoopCount >= 0)
			{
				stopLoop();
				break;
			}
		}
	}
}

void StartLoopCommand::initTokensList(ScriptLine * currentScriptLine, std::vector<std::vector<std::string>>& tokensList)
{
	// read the commands within the bound of the loop
	if (currentScriptLine->file.is_open() && !currentScriptLine->file.eof())
	{
		bool stop = false;
		while (!stop)
		{
			if (currentScriptLine->file.eof())
			{
				LOGGER->Log("StartLoopCommand", "EOF of file reached unexpectedly!");
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

				// if its an endloop command, stop reading
				if (cmdWord == "endloop")
				{
					stop = true;
				}
				// otherwise add it to the list
				else if (cmdWord != "")
				{
					tokensList.push_back(tokens);
				}

			}
		}
	}
	else
	{
		LOGGER->Log("StartLoopCommand", "Invalid script file!");
		return;
	}
}
