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

	// NOTE: We are assuming that none of the pointer will be duplicates!
	// if there is duplicate, return false
	bool addTexture(sf::Texture* tex, std::string path);
	bool addAudio(sf::SoundBuffer* audio, std::string path);

	// clear all queued items to be loaded (or done loaded)
	// if the loader has started but not yet done loading, will return false
	bool reset();

	// start loading the resources added to the list but not yet loaded
	void start();

	// return if all added items are being loaded
	bool doneLoading() const;

	// return if the loader has been started
	bool hasStarted() const;

	// join all threads that are initialized, meaning to wait till they finish executing
	void joinAll();

	// join a texture loader, meaning to wait till that texture has finished loading
	void join(sf::Texture* ptr);

	// join a soundbuffer loader, meaning to wait till that buffer has finished loading
	void join(sf::SoundBuffer* ptr);

	// Calculate the progress of the load, returns percentage in the range of 0 to 1
	// NOTE: this only counts by the number of files, without taking into account of the file size
	// NOTE: if there's one very large file not done loading, it can display (99%) 
	//       when all other small files are loaded
	float calcProgress() const;

private:

	// holds all the loaders
	std::map<void *, LoaderThread*> allLoaders;

	// holds pointers to the loaders not launched yet
	std::vector<LoaderThread*> queuedLoaders;

	// all the threads
	std::map< void *, std::thread > allThreads;

	bool started = false;

	// joinAll a thread
	void join(void* ptr);
};