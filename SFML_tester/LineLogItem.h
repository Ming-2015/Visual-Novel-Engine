#pragma once

class LineLogItem {
public:
	std::string name;
	std::string line;
	std::vector<std::string> voiceFile;
	std::string musicFile;
	
	std::string scriptFile;
	unsigned int scriptFilePos;

	std::set<std::string> flags;

	LineLogItem() {}

	LineLogItem(std::string name,
		std::string line,
		std::vector<std::string> voiceFile,
		std::string musicFile,
		std::string scriptFile,
		unsigned int scriptFilePos,
		std::set<std::string> flags)
		:
		name(name), 
		line(line), 
		voiceFile(voiceFile), 
		musicFile(musicFile), 
		scriptFile(scriptFile), 
		scriptFilePos(scriptFilePos),
		flags(flags)
	{}

};