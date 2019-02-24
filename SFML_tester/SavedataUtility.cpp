#include "SavedataUtility.h"

SavedataUtility * SavedataUtility::util_ptr = nullptr;

SavedataUtility::SavedataUtility()
{

}

bool SavedataUtility::readSave(const std::string & savefile, sf::Image & image, std::string & title) const
{
	ifstream infile(savefile, ios::binary | ios::in);
	if (!infile)
	{
		LOGGER->Log("SavedataUtility", "Unable to load save file");
		return false;
	}

	// read the image file size
	unsigned int fileSize;
	infile.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));

	// read the image data from file
	std::vector<char> byteArray(fileSize);
	infile.read(byteArray.data(), fileSize);

	// dump it into a memory input stream
	sf::MemoryInputStream picStream;
	picStream.open(byteArray.data(), fileSize);

	// load the image!
	image.loadFromStream(picStream);

	// read the title
	title = UTILITY->readFromBinaryFile(infile);

	// read the script manager
	infile.close();

	return true;
}

void SavedataUtility::writeSave(const std::string & filename, const sf::Image & image, const ScriptManager *& scriptManager) const
{
	ofstream outfile(filename, ios::binary | ios::out);
	if (!outfile)
	{
		LOGGER->Log("SavedataUtility", "Unable to create a new save file");
		return;
	}

	// create and read the screenshot
	image.saveToFile("_temp_ss.png");
	ifstream ssfile("_temp_ss.png", ios::binary | ios::in);

	// create a string of that size
	std::string str;
	ssfile.seekg(ssfile.end);
	str.reserve(ssfile.tellg());
	ssfile.seekg(ssfile.beg);

	// read everything from the file
	str.assign(std::istreambuf_iterator<char>(ssfile), std::istreambuf_iterator<char>());
	unsigned int fileSize = str.length();
	outfile.write(reinterpret_cast<const char*>(&fileSize), sizeof(fileSize));
	outfile.write(reinterpret_cast<const char*>(str.c_str()), fileSize);

	// remove screenshot
	ssfile.close();
	std::remove("_temp_ss.png");

	std::string title = scriptManager->getDisplayName();
	if (title == "")
	{
		title = scriptManager->getScriptLine();
	}
	else
	{
		title = title + ": " + scriptManager->getScriptLine();
	}

	// save the title of the script
	UTILITY->writeToBinaryFile(outfile, title);

	// save the scriptManager
	scriptManager->serialize(outfile);

	outfile.close();
}

bool SavedataUtility::readSave(const std::string & savefile, sf::Image & image, std::string & title, ScriptManager *& scriptManager) const
{
	ifstream infile(savefile, ios::binary | ios::in);
	if (!infile)
	{
		LOGGER->Log("SavedataUtility", "Unable to load save file");
		return false;
	}

	// read the image file size
	unsigned int fileSize;
	infile.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));

	// read the image data from file
	std::vector<char> byteArray(fileSize);
	infile.read(byteArray.data(), fileSize);

	// dump it into a memory input stream
	sf::MemoryInputStream picStream;
	picStream.open(byteArray.data(), fileSize);

	// load the image!
	image.loadFromStream(picStream);

	// read the title
	title = UTILITY->readFromBinaryFile(infile);

	// read the script manager
	scriptManager = new ScriptManager(infile);
	infile.close();

	return true;
}

SavedataUtility * SavedataUtility::GetUtility()
{
	if (util_ptr == nullptr)
	{
		util_ptr = new SavedataUtility();
	}

	return util_ptr;
}

void SavedataUtility::CleanUp()
{
	if (util_ptr != nullptr)
	{
		delete util_ptr;
	}
}
