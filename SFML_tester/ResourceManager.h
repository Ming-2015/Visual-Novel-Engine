#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>

#include "ResourceLoader.h"

#define RESOURCE ResourceManager::GetResourceManager()

class ResourceManager {
public:

	// add the asset paths, but don't load them yet!
	void addTexture(std::string path);
	void addAudio(std::string path);

	// add various asset paths, then start loading!
	void startLoading();

	// clear the resource loader IF everything is loaded, or IF it has not started loading
	// recommended to be used after checking 'doneLoading()' or 'join()'
	bool clear();

	// return if the resource loader has started loading things
	// NOTE: this will not be set to false even after doneLoading() returns true
	// For checking if the assets are finished loading, use doneLoading()
	bool hasStartedLoading() const;

	// check if all assets are being loaded
	bool doneLoading() const;

	// wait till all assets are done loading
	void joinAll();

	// join the thread that is loading the resource; use this to make sure a resource is fully loaded
	void join(std::string path); 

	// get the assets based on the paths... 
	// NOTE: the assets are not guaranteed to finish loading by now
	sf::Texture* getTexture(std::string path);
	sf::SoundBuffer* getAudio(std::string path);

	// unload all the current assets; useful for switching between game states
	void unloadAll();
	
	// unload specific files; useful when you know which file won't be needed anymore
	void unloadAudio(std::string path);
	void unloadTexture(std::string path);

	// singleton design
	static ResourceManager* GetResourceManager();
	static void CleanUp();

private:

	ResourceLoader resourceLoader;

	std::map<std::string, sf::Texture*> allTextures;
	std::map<std::string, sf::SoundBuffer*> allAudio;

	// default constructor/destructor
	ResourceManager();
	~ResourceManager();

	static ResourceManager* m_pThis;
};

ResourceManager* ResourceManager::m_pThis = nullptr;