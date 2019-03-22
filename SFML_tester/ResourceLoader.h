#pragma once

#include <map>
#include <vector>
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Logger.h"

class LoaderThread
{
public:
	virtual void startLoading() = 0;

	bool isDone() const { return doneLoading; }
	bool isValid() const { return valid; }
	bool hasStartedLoading() const { return startedLoading; }

protected:
	bool startedLoading = false;
	bool doneLoading = false;
	bool valid = false;
};

class TextureLoaderThread : public LoaderThread {
public:

	TextureLoaderThread(const std::string& path, sf::Texture* texPtr)
		:path(path), texPtr(texPtr) {}

	void startLoading() 
	{
		startedLoading = true;
		if (!texPtr->loadFromFile(path))
		{
			std::string err = "Failed to load texture: " + path;
			LOGGER->Log("TextureLoaderThread", err);
		}
		else
		{
			valid = true;
			texPtr->setSmooth(true);
		}
		doneLoading = true;
	}

	std::string path;
	sf::Texture* texPtr;
};

// For loading smaller audio, completely from file
// For loading BGM that needs a lot of space, use sf::Music
class AudioLoaderThread : public LoaderThread {
public:

	AudioLoaderThread(const std::string& path, sf::SoundBuffer* audioPtr)
		:path(path), audioPtr(audioPtr) {}

	void startLoading()
	{
		startedLoading = true;
		if (!audioPtr->loadFromFile(path))
		{
			std::string err = "Failed to load audio: " + path;
			LOGGER->Log("AudioLoaderThread", err);
		}
		else
		{
			valid = true;
		}
		doneLoading = true;
	}

	std::string path;
	sf::SoundBuffer* audioPtr;
};

class ResourceLoader {
public:

	ResourceLoader();
	~ResourceLoader();

	// NOTE: We are assuming that none of the path will be duplicates!
	void addTexture(sf::Texture* tex, std::string path);
	void addAudio(sf::SoundBuffer* audio, std::string path);

	// clear all queued items to be loaded (or done loaded)
	// if the loader has started but not yet done loading, will return false
	bool reset();

	// start loading the resources added to the list but not yet loaded
	void start();

	// return if all added items are being loaded
	bool doneLoading() const;

	// return if the loader has been started
	bool hasStarted() const;

	// joinAll all threads that are initialized
	void joinAll();
	void join(std::string path);

private:

	std::vector<TextureLoaderThread> textureLoaders;
	std::vector<AudioLoaderThread> audioLoaders;

	std::vector< std::pair< std::string, std::thread> > allThreads;

	bool started = false;
};