#pragma once
#include <ctype.h>
#include <stdio.h>
#include <iostream>
#include <map>

#include "ScriptCommand.h"

class StartLoopCommand : public ScriptCommand
{
public:

	StartLoopCommand(std::vector<std::string> args);
	~StartLoopCommand();

	// serialize the file
	StartLoopCommand(ifstream& savefile);
	void serialize(ofstream& savefile) const override;

	void execute(ScriptLine* scriptLine);
	void skipUpdate();
	void update(float delta_t);

	void stopLoop();

	static std::map<std::string, StartLoopCommand* > loopMap;

private:

	std::string objectTypeName;
	std::string flag;
	std::string loopName;	// the name of the character or background

	int flagType;
	int objectType;

	int totalNumLoop;		// number of loops (-1 for infinity)
	int loopCount = 0;
	int commandIdx = 0;

	bool initialized = false;
	std::vector< std::vector<std::string> > tokensList;
	std::vector<ScriptCommand*> executingCommandsList;

	const static int FLAG_WAIT = 1;
	const static int FLAG_NONE = 0;

	const static int OBJECT_LOOP = 0;

	bool doneAllCommands() const;
	void readCommands(ScriptLine* currentScriptLine, std::vector<ScriptCommand*>& executingCommandsList, int& commandIdx, int& loopCount, const int& maxLoopCount);
	void initTokensList(ScriptLine* currentScriptLine, std::vector<std::vector<std::string>>& tokensList);
};