#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "Logger.h"

using namespace std;

#define GLOBAL Global::GetGlobal()

class Global {
public:
	 
	static Global* GetGlobal();
	static void CleanUp();

	// Define all the global variables to be passed between states
	std::string MAIN_STATE_currentFile;
	unsigned int MAIN_STATE_currentLineId;
	int tempint;

private:
	Global();
	~Global();
	static Global * global_ptr;
};
