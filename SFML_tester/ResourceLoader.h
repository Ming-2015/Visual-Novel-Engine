#pragma once

#include <map>
#include <vector>
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Logger.h"
#include "LoaderThread.h"

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
	void join(sf::Texture* ptr);
	void join(sf::SoundBuffer* ptr);

private:

	// holds all the loaders
	std::vector<LoaderThread*> allLoaders;

	// holds pointers to the loaders not launched yet
	std::vector<LoaderThread*> queuedLoaders;

	// all the threads
	std::map< void *, std::thread > allThreads;

	bool started = false;

	// joinAll a thread
	void join(void* ptr);
};