#include "ScriptCommand.h"

ScriptCommand::ScriptCommand(std::vector<std::string> args)
	: args(args)
{
}

ScriptCommand::ScriptCommand(ifstream & savefile)
{
	try
	{
		savefile.read(reinterpret_cast<char *> (&commandType), sizeof(commandType));
		savefile.read(reinterpret_cast<char *> (&wait), sizeof(wait));
		savefile.read(reinterpret_cast<char *> (&valid), sizeof(valid));
		savefile.read(reinterpret_cast<char *> (&done), sizeof(done));
		args = UTILITY->readVectorFromBinaryFile(savefile);
	}
	catch (exception e)
	{
		LOGGER->Log("ScriptCommand", "Unable to read blur command");
		valid = false;
		return;
	}
}

void ScriptCommand::serialize(ofstream & savefile) const
{
	savefile.write(reinterpret_cast<const char *> (&commandType), sizeof(commandType));
	savefile.write(reinterpret_cast<const char *> (&wait), sizeof(wait));
	savefile.write(reinterpret_cast<const char *> (&valid), sizeof(valid));
	savefile.write(reinterpret_cast<const char *> (&done), sizeof(done));
	UTILITY->writeVectorToBinaryFile(savefile, args);
}
