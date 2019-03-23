#pragma once

#include <map>
#include <vector>
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Logger.h"
#include "LoaderThread.h"
#include "SavedataReader.h"\

class ResourceLoader {
public:

	ResourceLoader();
	~ResourceLoader();

	// Add texture to the loader, but don't load them yet!
	// NOTE: We are assuming that none of the pointer will be duplicates!
	// if there is duplicate, return false
	bool addTexture(sf::Texture* tex, std::string path);

	// Add audio to the loader, but don't load them yet!
	// NOTE: We are assuming that none of the pointer will be duplicates!
	// if there is duplicate, return false
	bool addAudio(sf::SoundBuffer* audio, std::string path);

	// Add font to the loader, but don't load them yet!
	// NOTE: We are assuming that none of the pointer will be duplicates!
	// if there is duplicate, return false
	bool addFont(sf::Font* font, std::string path);

	// Add savedata to the loader, but don't load them yet!
	// NOTE: We are assuming that none of the pointer will be duplicates!
	// if there is duplicate, return false
	bool addSavedata(SavedataReader* savedata, std::string path);

	// clear all queued items to be loaded (or done loaded)
	// if the loader has started but not yet done loading, will return false
	bool reset();

	// start loading ALL the resources added to the list but not yet loaded
	void startAll();

	// start loading a specific texture that's previously added
	bool startTexture(sf::Texture* tex);

	// start loading a specific audio that's previously added
	bool startAudio(sf::SoundBuffer* audio);

	// start loading a specific font that's previously added
	bool startFont(sf::Font* font);

	// start loading a specific texture that's previously added
	bool startSavedata(SavedataReader* savedata);

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

	// join a font loader, meaning to wait till the font has finished loading
	void join(sf::Font* ptr);

	// join a savedata loader, meaning to wait till the font has finished loading
	void join(SavedataReader* ptr);

	// Calculate the progress of the load, returns percentage in the range of 0 to 1
	// NOTE: this only counts by the number of files, without taking into account of the file size
	// NOTE: if there's one very large file not done loading, it can display (99%) 
	//       when all other small files are loaded
	float calcProgress() const;

private:

	// holds all the loaders
	std::map<void *, LoaderThread*> allLoaders;

	// holds pointers to the loaders not launched yet
	std::map<void *, LoaderThread*> queuedLoaders;

	// all the threads
	std::map<void *, std::thread > allThreads;

	bool started = false;

	// joinAll a thread
	void join(void* ptr);
	bool start(void* ptr);
};