#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <chrono>
#include <time.h>
#include <stdio.h>
#undef _CRT_SECURE_NO_WARNINGS

#include "ScriptManager.h"

#define SAVEDATAUTILITY SavedataUtility::GetUtility()

class SavedataUtility
{
public:

	const std::string SavefileRoot = "saves/";
	const std::string SavefilePrefix = "save_";
	const std::string SavefileSuffix = ".usd";

	const std::string QuickSavefilePrefix = "qsave_";
	const std::string QuickSavefileSuffix = ".usd";

	bool readSave(const std::string & savefile, sf::Image & image, std::string & title,
		std::string & savetime) const;
	void writeSave(const std::string& filename,
		const sf::Image & image,
		const ScriptManager*& scriptManager) const;

	// Warning: this function creates a new scriptManager, which will not be deallocated locally
	bool readSave(const std::string& savefile,
		sf::Image& image,
		std::string& title,
		std::string& time,
		ScriptManager*& scriptManager) const;

	static SavedataUtility* GetUtility();
	static void CleanUp();

private:

	SavedataUtility();
	static SavedataUtility * util_ptr;

};