#pragma once
#include <string>
#include <vector>

#include "scriptLine.h"
#include "Utility.h"
#include "ScriptCommandFactory.h"

class ScriptCommand
{
public:

	ScriptCommand(std::vector<std::string> args);

	// serialize the file
	ScriptCommand(ifstream& savefile);
	virtual void serialize(ofstream& savefile) const;

	// Make the changes to the scriptline object
	virtual void execute(ScriptLine* scriptLine) = 0;

	// Clean up all the objects in this command
	virtual void skipUpdate() = 0;

	// Update the changes as a function of time
	virtual void update(float delta_t) = 0;

	// Check if the arguments passed into the constructor is valid
	bool validArgs() { return valid; }

	// Check if you should wait for this command to finish executing
	bool shouldWait() { return wait; }
	
	// Check if the command is done executing; delete if done in script manager
	bool isDone() { return done; }

	// get the command type of this command
	int getCommandType() const { return commandType; }

	static const int COMMAND_INVALID = -1;
	static const int COMMAND_SHOW = 0;
	static const int COMMAND_MOVE = 1;
	static const int COMMAND_ZOOM = 2;
	static const int COMMAND_ROTATE = 3;
	static const int COMMAND_REMOVE = 4;
	static const int COMMAND_PLAY = 5;
	static const int COMMAND_STOP = 6;
	static const int COMMAND_PAUSE = 7;
	static const int COMMAND_RESUME = 8;
	static const int COMMAND_DISPLAY = 9;
	static const int COMMAND_CLEAR = 10;
	static const int COMMAND_JUMP = 11;
	static const int COMMAND_SET = 12;
	static const int COMMAND_UNHIDE = 13;
	static const int COMMAND_HIDE = 14;
	static const int COMMAND_DELAY = 15;
	static const int COMMAND_BLUR = 16;
	static const int COMMAND_FLASH = 17;
	static const int COMMAND_STARTLOOP = 18;
	static const int COMMAND_ENDLOOP = 19;
	static const int COMMAND_STOPLOOP = 20;
	static const int COMMAND_BREAKLOOP = 21;
	static const int COMMAND_CONTINUELOOP = 22;
	static const int COMMAND_CLEARLOOP = 23;
	static const int COMMAND_ANCHOR = 24;
	static const int COMMAND_START = 25;
	static const int COMMAND_END = 26;

	static const int COLUMN_ACTION = 0;
	static const int COLUMN_OBJECT = 1;
	static const int COLUMN_FLAG = 2;
	static const int COLUMN_ARG1 = 3;
	static const int COLUMN_ARG2 = 4;
	static const int COLUMN_ARG3 = 5;
	static const int COLUMN_ARG4 = 6;
	static const int COLUMN_ARG5 = 7;
	static const int COLUMN_ARG6 = 8;
	static const int COLUMN_ARG7 = 9;

protected:

	int commandType = -1;
	bool wait = false;
	bool valid = true;
	bool done = false;
	std::vector<std::string> args;

private:

};