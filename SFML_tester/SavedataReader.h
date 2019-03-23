#pragma once

#include <SFML/Graphics.hpp>
#include "ScriptManager.h"
#include "SavedataUtility.h"

// Wrapper class for reading savedata 
class SavedataReader {
public:

	SavedataReader() { }
	~SavedataReader() { }

	bool loadFromFile(std::string path, bool readScriptManager = false);
	bool isLoaded() const;

	void cleanScriptManager();

	const sf::Image& getSaveImage() const;
	std::string getTitleStr() const;
	std::string getDateStr() const;
	std::string getFilePath() const;
	ScriptManager* getScriptManager() const;

private:

	sf::Image saveImage;
	ScriptManager* scriptManager = nullptr;
	std::string title;
	std::string date;
	std::string filepath;
	bool loaded = false;
};