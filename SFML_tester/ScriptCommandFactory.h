#pragma once
#include <fstream>
#include <vector>

class ScriptCommand;

// NOTE: All commands generated here are dynamic and not deallocated by itself
// Must handle deallocation manually.
class ScriptCommandFactory {
public:

	// generate new command by the list of tokens
	static void GenerateCommandByTokens(std::vector<std::string> tokens,
		ScriptCommand*& command, int& commandType);

	// reproduce old command by reading deep copy on binary file 
	static void GenerateCommandByFile(std::ifstream& savefile,
		ScriptCommand*& command, int& commandType);
};