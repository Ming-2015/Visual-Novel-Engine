#pragma once
#include <ctype.h>
#include <stdio.h>
#include <iostream>

#include "ScriptCommand.h"

class ShowCommand : public ScriptCommand
{
public:

	ShowCommand(std::vector<std::string> args);
	~ShowCommand();

	// serialize the file
	ShowCommand(ifstream& savefile);
	void serialize(ofstream& savefile) const override;

	void execute(ScriptLine* scriptLine);
	void skipUpdate();
	void update(float delta_t);

private:
	
	std::string objectTypeName;
	std::string flag;

	std::string objectName;			// the name of the character or background
	std::string objectSubname;		// the expression of character or time-of-the-day of background

	float x1, y1;
	float time;
	bool initialized = false;

	int animationType;
	float alpha;

	int objectType;

	const static int ANIMATION_FADEIN = 1;
	const static int ANIMATION_NONE = 0;

	const static int OBJECT_CHARACTER = 0;
	const static int OBJECT_BACKGROUND = 1;
};