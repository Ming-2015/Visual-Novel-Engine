#pragma once

#include "ScriptCommand.h"

class ResumeCommand : public ScriptCommand
{
public:

	ResumeCommand(std::vector<std::string> args);
	~ResumeCommand();

	// serialize the file
	ResumeCommand(ifstream& savefile);
	void serialize(ofstream& savefile) const override;

	void execute(ScriptLine* scriptLine);
	void skipUpdate();
	void update(float delta_t);

private:

	std::string objectTypeName;
	std::string flag;

	float time;
	float finalVolume;
	float currentVolume = 0;

	int flagType;
	int objectType;
	bool finishedAction = false;
	bool played = false;

	const static int FLAG_FADEIN = 1;
	const static int FLAG_NONE = 0;

	const static int OBJECT_BGM = 0;
	const static int OBJECT_VOICE = 1;
	const static int OBJECT_SFX = 2;
	const static int OBJECT_ALL = 3;
};