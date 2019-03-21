#pragma once
#include <map>
#include "ScriptCommand.h"

class StopLoopCommand : public ScriptCommand
{
public:

	StopLoopCommand(std::vector<std::string> args);
	~StopLoopCommand();

	// serialize the file
	StopLoopCommand(ifstream& savefile);
	void serialize(ofstream& savefile) const override;

	void execute(ScriptLine* scriptLine);
	void skipUpdate();
	void update(float delta_t);

private:

	std::string objectTypeName;
	std::string flag;
	std::string loopName;	// the name of the character or background
	std::string flagExpression;

	int flagType;
	int objectType;

	const static int FLAG_PARSE = 1;
	const static int FLAG_NONE = 0;

	const static int OBJECT_LOOP = 0;
};