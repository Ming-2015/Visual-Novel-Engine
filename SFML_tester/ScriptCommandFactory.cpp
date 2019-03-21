#include "ScriptCommandFactory.h"

#include "ScriptCommand.h"
#include "ScriptLine.h"

#include "ShowCommand.h"
#include "DisplayCommand.h"
#include "SetCommand.h"
#include "MoveCommand.h"
#include "RotateCommand.h"
#include "ZoomCommand.h"
#include "PlayCommand.h"
#include "StopCommand.h"
#include "PauseCommand.h"
#include "ResumeCommand.h"
#include "RemoveCommand.h"
#include "ClearCommand.h"
#include "FlashCommand.h"
#include "HideCommand.h"
#include "DelayCommand.h"
#include "UnhideCommand.h"
#include "JumpCommand.h"
#include "BlurCommand.h"
#include "StartLoopCommand.h"
#include "StopLoopCommand.h"

void ScriptCommandFactory::GenerateCommandByTokens(std::vector<std::string> tokens, ScriptCommand *& command, int & commandType)
{
	std::string cmdWord = UTILITY->toLower(tokens[0]);
	command = nullptr;
	commandType = ScriptCommand::COMMAND_INVALID;

	if (cmdWord == "show")
	{
		command = new ShowCommand(tokens);
	}
	else if (cmdWord == "remove")
	{
		command = new RemoveCommand(tokens);
	}
	else if (cmdWord == "clear")
	{
		command = new ClearCommand(tokens);
	}
	else if (cmdWord == "flash")
	{
		command = new FlashCommand(tokens);
	}
	else if (cmdWord == "blur")
	{
		command = new BlurCommand(tokens);
	}
	else if (cmdWord == "display")
	{
		command = new DisplayCommand(tokens);
	}
	else if (cmdWord == "set")
	{
		command = new SetCommand(tokens);
	}
	else if (cmdWord == "move")
	{
		command = new MoveCommand(tokens);
	}
	else if (cmdWord == "rotate")
	{
		command = new RotateCommand(tokens);
	}
	else if (cmdWord == "zoom")
	{
		command = new ZoomCommand(tokens);
	}
	else if (cmdWord == "play")
	{
		command = new PlayCommand(tokens);
	}
	else if (cmdWord == "stop")
	{
		command = new StopCommand(tokens);
	}
	else if (cmdWord == "pause")
	{
		command = new PauseCommand(tokens);
	}
	else if (cmdWord == "resume")
	{
		command = new ResumeCommand(tokens);
	}
	else if (cmdWord == "hide")
	{
		command = new HideCommand(tokens);
	}
	else if (cmdWord == "delay")
	{
		command = new DelayCommand(tokens);
	}
	else if (cmdWord == "unhide")
	{
		command = new UnhideCommand(tokens);
	}
	else if (cmdWord == "jump" && tokens.size() >= 4)
	{
		command = new JumpCommand(tokens);
	}
	else if (cmdWord == "startloop")
	{
		command = new StartLoopCommand(tokens);
	}
	else if (cmdWord == "endloop")
	{
		command = nullptr;
		commandType = ScriptCommand::COMMAND_ENDLOOP;
	}
	else if (cmdWord == "breakloop")
	{
		command = nullptr;
		commandType = ScriptCommand::COMMAND_BREAKLOOP;
	}
	else if (cmdWord == "continueloop")
	{
		command = nullptr;
		commandType = ScriptCommand::COMMAND_CONTINUELOOP;
	}
	else if (cmdWord == "clearloop")
	{
		command = nullptr;
		commandType = ScriptCommand::COMMAND_CLEARLOOP;
	}
	else if (cmdWord == "stoploop")
	{
		command = new StopLoopCommand(tokens);
	}
	else if (cmdWord == "anchor")
	{
		commandType = ScriptCommand::COMMAND_ANCHOR;
		command = nullptr;
	}
	else if (cmdWord != "")
	{
		string msg = "Invalid Command found: " + cmdWord;
		LOGGER->Log("ScriptCommandFactory", msg);

		commandType = ScriptCommand::COMMAND_INVALID;
		command = nullptr;
	}

	if (command != nullptr)
	{
		commandType = command->getCommandType();
	}
}

void ScriptCommandFactory::GenerateCommandByFile(std::ifstream & savefile, ScriptCommand *& command, int & commandType)
{
	int prevPos = savefile.tellg();
	savefile.read(reinterpret_cast<char*> (&commandType), sizeof(commandType));
	savefile.seekg(prevPos);
	switch (commandType)
	{
		case ScriptCommand::COMMAND_BLUR:
		{
			command = new BlurCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_CLEAR:
		{
			command = new ClearCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_DELAY:
		{
			command = new DelayCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_DISPLAY:
		{
			command = new DisplayCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_FLASH:
		{
			command = new FlashCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_HIDE:
		{
			command = new HideCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_JUMP:
		{
			command = new JumpCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_MOVE:
		{
			command = new MoveCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_PAUSE:
		{
			command = new PauseCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_PLAY:
		{
			command = new PlayCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_REMOVE:
		{
			command = new RemoveCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_RESUME:
		{
			command = new ResumeCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_ROTATE:
		{
			command = new RotateCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_SET:
		{
			command = new SetCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_SHOW:
		{
			command = new ShowCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_STOP:
		{
			command = new StopCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_UNHIDE:
		{
			command = new UnhideCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_ZOOM:
		{
			command = new ZoomCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_STARTLOOP:
		{
			command = new StartLoopCommand(savefile);
			break;
		}
		case ScriptCommand::COMMAND_STOPLOOP:
		{
			command = new StopLoopCommand(savefile);
			break;
		}
		default:
		{
			std::string err = "Found an invalid command type code: " + commandType;
			LOGGER->Log("ScriptCommandFactory", err);
			command = nullptr;
			break;
		}
	}
}
