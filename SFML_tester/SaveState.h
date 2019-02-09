#pragma once
#include "scriptLine.h"
#include "Utility.h"
#include "charPic.h"
#include "logger.h"
#include "GameState.h"
#include "ScriptLine.h"
#include "ScriptManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

class SaveState {
public:
	SaveState();
	int lineIDFromSave;
	string fileNameFromSave;
	void writeSave();
	void readSave();

private:
	ScriptManager* scriptManager;    //maybe take out
	string scriptfileName;
	unsigned int lineId;
	string tempStr;
};