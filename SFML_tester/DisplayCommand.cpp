#include "DisplayCommand.h"

DisplayCommand::DisplayCommand(std::vector<std::string> args)
	: ScriptCommand(args)
{
	// checking if the first argument is show
	if (UTILITY->toLower(args[COLUMN_ACTION]) != "display")
	{
		LOGGER->Log("DisplayCommand", "Invalid Command Input");
		valid = false;
		return;
	}
	commandType = COMMAND_DISPLAY;

	if (args.size() < 5)
	{
		LOGGER->Log("DisplayCommand", "Too few arguments");
		valid = false;
		return;
	}

	// parse the arguments
	objectTypeName = UTILITY->toLower(args[COLUMN_OBJECT]);	// column 2 : object type character or background
	flag = UTILITY->toLower(args[COLUMN_FLAG]);				// column 3 : flag

	if (objectTypeName == "line" || objectTypeName == "l" || objectTypeName == "")
	{
		objectType = OBJECT_LINE;
	}
	else if (objectTypeName == "choice" || objectTypeName == "c")
	{
		objectType = OBJECT_CHOICE;
	}
	else if (objectTypeName == "voicedline" || objectTypeName == "v" || objectTypeName == "vl" ||
		objectTypeName == "voiced" || objectTypeName == "voice" || objectTypeName == "voiceline")
	{
		objectType = OBJECT_VOICED_LINE;
	}
	else
	{
		string err = "Invalid Object Type: " + objectTypeName;
		LOGGER->Log("DisplayCommand", err);
		valid = false;
		return;
	}

	// parse the flag
	if (flag == "none" || flag == "n" || flag == "" || flag == "default")
	{
		animationType = ANIMATION_NONE;
	}
	else if (flag == "exclaimation" || flag == "e" || flag == "ex")
	{
		animationType = ANIMATION_EXCLAIMATION;
	}
	else if (flag == "gorey" || flag == "g")
	{
		animationType = ANIMATION_GOREY;
	}
	else if (flag == "instant" || flag == "i")
	{
		animationType = ANIMATION_INSTANT;
	}
	else
	{
		string err = "Invalid Flag: " + flag;
		LOGGER->Log("DisplayCommand", err);
		valid = false;
		return;
	}

	displayName = args[COLUMN_ARG1];		// column 4 : character name, also work as num of choices

	if (objectType == OBJECT_LINE)
	{
		for (int i = COLUMN_ARG2; i < COLUMN_ARG7; i++)
		{
			if (args[i].empty()) break;

			displayLines.push_back(UTILITY->replaceAllSubstrings(args[i], "[player]", GLOBAL->playerName));
		}
	}
	else if (objectType == OBJECT_CHOICE)
	{
		int numChoices = 0;
		try 
		{
			numChoices = stoi(displayName);
		}
		catch (exception e)
		{
			LOGGER->Log("DisplayCommand", "Invalid Number of Choices");
			valid = false;
			return;
		}

		bool choice = true;
		for (int i = COLUMN_ARG2; i < COLUMN_ARG2 + 2 * numChoices; i++)
		{
			if (args.size() <= i)
			{
				LOGGER->Log("DisplayCommand", "Missing choices and/or flags");
				valid = false;
				return;
			}

			if (choice)
			{
				displayLines.push_back( UTILITY->replaceAllSubstrings( args[i], "[player]", GLOBAL->playerName) );
			}
			else
			{
				userFlags.push_back(args[i]);
			}
			choice = !choice;
		}
	}
	// alternatively adding line and voice file
	else if (objectType == OBJECT_VOICED_LINE)
	{
		int i = COLUMN_ARG2;
		bool isLine = true;
		while (i < args.size() && !args[i].empty())
		{
			if (isLine)
			{
				displayLines.push_back(UTILITY->replaceAllSubstrings(args[i], "[player]", GLOBAL->playerName));
			}
			else
			{
				voiceFiles.push_back(args[i]);
			}
			isLine = !isLine;	// toggle isLine
			i++;
		}
	}

	wait = true;
	done = false;
	valid = true;
	currentLine = "";
	timer = 0;
	currentLineIndex = 0;
	currentCharIndex = 0;

	if (animationType == ANIMATION_INSTANT && objectType == OBJECT_LINE)
	{
		currentLineIndex = displayLines.size() - 1;
		currentCharIndex = displayLines[currentLineIndex].length() - 1;
		currentLine = assembleString(displayLines, currentLineIndex, currentCharIndex);
	}
}

DisplayCommand::~DisplayCommand()
{
}

DisplayCommand::DisplayCommand(ifstream & savefile)
	:ScriptCommand(savefile)
{
	try {

		objectTypeName = UTILITY->readFromBinaryFile(savefile);
		flag = UTILITY->readFromBinaryFile(savefile);
		displayName = UTILITY->readFromBinaryFile(savefile);
		displayLines = UTILITY->readVectorFromBinaryFile(savefile);
		userFlags = UTILITY->readVectorFromBinaryFile(savefile);
		voiceFiles = UTILITY->readVectorFromBinaryFile(savefile);

		savefile.read(reinterpret_cast<char *> (&animationType), sizeof(animationType));
		savefile.read(reinterpret_cast<char *> (&objectType), sizeof(objectType));

		currentLine = UTILITY->readFromBinaryFile(savefile);

		savefile.read(reinterpret_cast<char *> (&timer), sizeof(timer));
		savefile.read(reinterpret_cast<char *> (&currentLineIndex), sizeof(currentLineIndex));
		savefile.read(reinterpret_cast<char *> (&currentCharIndex), sizeof(currentCharIndex));
		savefile.read(reinterpret_cast<char *> (&displayedChoices), sizeof(displayedChoices));
		savefile.read(reinterpret_cast<char *> (&selectedChoice), sizeof(selectedChoice));
		savefile.read(reinterpret_cast<char *> (&doneAddingToLog), sizeof(doneAddingToLog));
	}
	catch (exception e)
	{
		LOGGER->Log("DisplayCommand", "unable to read display command from save file");
		valid = false;
		return;
	}

	// reset the display lines
	autoForwardTimer = 0;
	currentLineIndex = 0;
	currentCharIndex = 0;
	currentLine = "";
	timer = 0;
	doneAddingToLog = false;
}

void DisplayCommand::serialize(ofstream & savefile) const
{
	ScriptCommand::serialize(savefile);

	UTILITY->writeToBinaryFile(savefile, objectTypeName);
	UTILITY->writeToBinaryFile(savefile, flag);
	UTILITY->writeToBinaryFile(savefile, displayName);
	UTILITY->writeVectorToBinaryFile(savefile, displayLines);
	UTILITY->writeVectorToBinaryFile(savefile, userFlags);
	UTILITY->writeVectorToBinaryFile(savefile, voiceFiles);

	savefile.write(reinterpret_cast<const char *> (&animationType), sizeof(animationType));
	savefile.write(reinterpret_cast<const char *> (&objectType), sizeof(objectType));

	UTILITY->writeToBinaryFile(savefile, currentLine);

	savefile.write(reinterpret_cast<const char *> (&timer), sizeof(timer));
	savefile.write(reinterpret_cast<const char *> (&currentLineIndex), sizeof(currentLineIndex));
	savefile.write(reinterpret_cast<const char *> (&currentCharIndex), sizeof(currentCharIndex));
	savefile.write(reinterpret_cast<const char *> (&displayedChoices), sizeof(displayedChoices));
	savefile.write(reinterpret_cast<const char *> (&selectedChoice), sizeof(selectedChoice));
	savefile.write(reinterpret_cast<const char *> (&doneAddingToLog), sizeof(doneAddingToLog));
}

void DisplayCommand::execute(ScriptLine * scriptLine)
{
	if (valid)
	{
		if (objectType == OBJECT_LINE || objectType == OBJECT_VOICED_LINE)
		{
			scriptLine->setDialogue(displayName, currentLine);

			if (!doneAddingToLog)
			{
				doneAddingToLog = true;
				scriptLine->appendLineToLog(
					displayName,
					assembleString(displayLines, displayLines.size() - 1, displayLines[displayLines.size() - 1].length() - 1),
					voiceFiles
				);
			}

			if (GLOBAL->autoMode)
			{
				if (scriptLine->isVoicePlayed())
				{
					shouldAdvance = true;
				}
				else
				{
					shouldAdvance = false;
				}
			}

			if (objectType == OBJECT_VOICED_LINE && !playedVoice && currentLineIndex < voiceFiles.size())
			{
				scriptLine->setVoice(".", voiceFiles[currentLineIndex]);
				playedVoice = true;
			}

			if (done && CONFIG->stopVoiceNextLine)
			{
				scriptLine->stopVoice();
			}
		}
		else if (objectType == OBJECT_CHOICE)
		{
			if (!displayedChoices)
			{
				displayedChoices = true;
				scriptLine->setChoices(displayLines, userFlags);
			}

			// if it is not a choice anymore, then user must have clicked a choice
			selectedChoice = !scriptLine->isChoice;
		}
	}
	else
	{
		done = true;
		wait = false;
		return;
	}
}

void DisplayCommand::skipUpdate()
{
	if (valid)
	{
		if (displayLines.size() <= 0)
		{
			wait = false;
			done = true;
		}

		if (objectType == OBJECT_CHOICE && animationType == ANIMATION_NONE)
		{
			if (selectedChoice)
			{
				wait = false;
				done = true;
			}
		}

		else if (objectType == OBJECT_LINE || objectType == OBJECT_VOICED_LINE)
		{
			// reset timer for auto mode
			if (GLOBAL->autoMode)
			{
				autoForwardTimer = 0;
				shouldAdvance = false;
			}

			if (animationType == ANIMATION_NONE)
			{
				if (currentLineIndex < displayLines.size())
				{
					if (currentCharIndex < displayLines[currentLineIndex].length() - 1)
					{
						currentCharIndex = displayLines[currentLineIndex].length() - 1;
					}
					else
					{
						currentLineIndex++;
						currentCharIndex = 0;

						// move on to the next voice
						playedVoice = false;
					}

					if (currentLineIndex < displayLines.size())
					{
						currentLine = assembleString(displayLines, currentLineIndex, currentCharIndex);
					}
					else
					{
						wait = false;
						done = true;
					}
				}
			}

			else if (animationType == ANIMATION_INSTANT)
			{
				wait = false;
				done = true;
			}
		}
		else
		{
			wait = false;
			done = true;
		}
	}
	else
	{
		wait = false;
		done = true;
	}
}

void DisplayCommand::update(float delta_t)
{
	if (objectType == OBJECT_LINE || objectType == OBJECT_VOICED_LINE)
	{

		if (currentLineIndex >= displayLines.size() 
			|| currentCharIndex >= displayLines[currentLineIndex].length() - 1)
		{
			if (GLOBAL->autoMode && shouldAdvance)
			{
				autoForwardTimer += delta_t;
				float forwardingTime = CONFIG->autoTextWaitTime * 8; // 0 - 8 sec wait time

				if (autoForwardTimer > forwardingTime)
				{
					skipUpdate();
				}
			}

			return;
		}

		if (animationType == ANIMATION_NONE)
		{
			float interval;
			if (GLOBAL->autoMode)
			{
				if (CONFIG->autoTextSpeed >= 1.0f)
				{
					currentCharIndex = displayLines[currentLineIndex].length() - 1;
					currentLine = assembleString(displayLines, currentLineIndex, currentCharIndex);
					return;
				}

				interval = 1.0f / (CONFIG->autoTextSpeed* 9.f / 10.f + .1f) - 1.0f;
			}
			else
			{
				if (CONFIG->manualTextSpeed >= 1.0f)
				{
					currentCharIndex = displayLines[currentLineIndex].length() - 1;
					currentLine = assembleString(displayLines, currentLineIndex, currentCharIndex);
					return;
				}

				interval = 1.0f / (CONFIG->manualTextSpeed * 9.f / 10.f + .1f) - 1.0f;
			}

			interval *= .03f;

			timer += delta_t;
			if (timer > interval)
			{
				int numChars = (int)(timer / interval);
				timer -= numChars * interval;
				currentCharIndex += numChars;	// note currentCharIndex might exceed actual length

				// boundary checks
				if (currentLineIndex < displayLines.size() && 
					currentCharIndex >= displayLines[currentLineIndex].length())
				{
					currentCharIndex = displayLines[currentLineIndex].length() - 1;
				}

				currentLine = assembleString(displayLines, currentLineIndex, currentCharIndex);
			}
		}
	}
}

std::string DisplayCommand::getFullLine() const
{
	return assembleString(displayLines, displayLines.size() - 1, displayLines[currentLineIndex].length() - 1);
}

std::string DisplayCommand::getName() const
{
	return displayName;
}

bool DisplayCommand::isLine() const
{
	return objectType == OBJECT_LINE;
}

std::string DisplayCommand::assembleString(const std::vector<std::string>& lines, int lineIndex, int charIndex) const
{
	std::string str = "";
	for (int i = 0; i < lineIndex; i++)
	{
		str += lines[i];
		str += " ";
	}
	str += lines[lineIndex].substr(0, charIndex + 1);
	return str;
}
