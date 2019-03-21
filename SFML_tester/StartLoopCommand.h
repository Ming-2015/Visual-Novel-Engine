#pragma once
#include <ctype.h>
#include <stdio.h>
#include <iostream>
#include <map>

#include "ScriptCommand.h"

#include "ShowCommand.h"
#include "DisplayCommand.h"
#include "SetCommand.h"
#include "MoveCommand.h"
#include "RotateCommand.h"
#include "ZoomCommand.h"
#include "PlayCommand.h"
#include "StopCommand.h"
#include "PauseCommand.h"
#include "ResumeCommand.h"
#include "RemoveCommand.h"
#include "ClearCommand.h"
#include "FlashCommand.h"
#include "HideCommand.h"
#include "DelayCommand.h"
#include "UnhideCommand.h"
#include "JumpCommand.h"
#include "BlurCommand.h"
#include "StopLoopCommand.h"

class StartLoopCommand : public ScriptCommand
{
public:

	StartLoopCommand(std::vector<std::string> args, ScriptLine* currentScriptLine);
	~StartLoopCommand();

	// serialize the file
	StartLoopCommand(ifstream& savefile, ScriptLine* currentScriptLine);
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

	std::vector< std::vector<std::string> > tokensList;
	std::vector<ScriptCommand*> executingCommandsList;

	const static int FLAG_WAIT = 1;
	const static int FLAG_NONE = 0;

	const static int OBJECT_LOOP = 0;

	bool doneAllCommands() const;
	void readCommands(ScriptLine* currentScriptLine, int& commandIdx, int& loopCount, const int& maxLoopCount);
};