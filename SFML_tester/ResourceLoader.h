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

protected:
	bool doneLoading = false;
	bool valid = false;
};

class TextureLoaderThread : public LoaderThread {
public:

	TextureLoaderThread(const std::string& path, sf::Texture* texPtr)
		:path(path), texPtr(texPtr) {}

	void startLoading() 
	{
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

class AudioLoaderThread : public LoaderThread {
public:

	AudioLoaderThread(const std::string& path, sf::Music* audioPtr)
		:path(path), audioPtr(audioPtr) {}

	void startLoading()
	{
		if (!audioPtr->openFromFile(path))
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
	sf::Music* audioPtr;
};

class ResourceLoader {
public:

	ResourceLoader();
	~ResourceLoader();

	void addTexture(sf::Texture* tex, std::string path);
	void addAudio(sf::Music* audio, std::string path);

	bool reset();
	void start();
	bool doneLoading();
	bool hasStarted();
	void join();

private:

	std::vector<TextureLoaderThread> TextureLoaders;
	std::vector<AudioLoaderThread> AudioLoaders;

	std::vector<std::thread> allThreads;

	bool started = false;
};