#pragma once
#include "ScriptCommand.h"

class JumpCommand : public ScriptCommand
{
public:

	JumpCommand(std::vector<std::string> args);
	~JumpCommand();

	void execute(ScriptLine* scriptLine);
	void skipUpdate();
	void update(float delta_t);

private:

	std::string objectTypeName;
	std::string flag;

	int conditionType;
	int objectType;

	std::string flagExpression;
	std::string filename;
	std::string anchor;

	const static int CONDITION_NONE = 0;
	const static int CONDITION_FLAG = 1;

	const static int OBJECT_ANCHOR = 1;
	const static int OBJECT_FILE = 0;

};