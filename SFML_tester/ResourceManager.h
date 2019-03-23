#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>

#include "ResourceLoader.h"

#define RESOURCE ResourceManager::GetResourceManager()

class ResourceManager {
public:

	// add the texture path, but don't load them yet!
	bool addTexture(std::string path);

	// add the audio path, but don't load them yet!
	bool addAudio(std::string path);

	// add the font path
	bool addFont(std::string path);

	// add the savedata path
	bool addSavedata(std::string path);

	// add various asset paths, then start loading!
	void startLoading();

	// clear the resource loader IF everything is loaded, or IF it has not started loading
	// recommended to be used after checking 'doneLoading()' or 'joinAll()'
	// note that this will clear all assets that have not started the loading process
	// return if the loader is successfully cleared
	bool clear();

	// return if the resource loader has started loading things
	// NOTE: this will not be set to false even after doneLoading() returns true
	// For checking if the assets are finished loading, use doneLoading()
	bool hasStartedLoading() const;

	// check if all assets are being loaded
	bool doneLoading() const;

	// join all the threads that are loading the resources 
	// use this to wait till all resources are fully loaded
	void joinAll();

	// use this to join an asset, whichever it might be (texture, audio, etc)
	void join(std::string path); 

	// use this to wait till the particular audio is fully loaded
	void joinAudio(std::string path);

	// use this to wait till the particular texture is fully loaded
	void joinTexture(std::string path);

	// use this to wait till the particular font is fully loaded
	void joinFont(std::string path);

	// use this to wait till the particular savedata is fully loaded
	void joinSavedata(std::string path);

	// Get the texture based on the paths 
	// NOTE: the texture are not guaranteed to finish loading by now
	sf::Texture* getTexture(std::string path);

	// Get the sound buffer based on the paths 
	// NOTE: the audio is not guaranteed to finish loading by now
	sf::SoundBuffer* getAudio(std::string path);

	// Get the sound buffer based on the paths 
	// NOTE: the font are not guaranteed to finish loading by now
	sf::Font* getFont(std::string path);

	// Get the sound buffer based on the paths 
	// NOTE: the savedata is not guaranteed to finish loading by now
	SavedataReader* getSavedata(std::string path);

	// unload all the current assets; useful for switching between game states
	void unloadAll();
	
	// unload specific audio; useful when you know which file won't be needed anymore
	void unloadAudio(std::string path);

	// unload specific texture; useful when you know which file won't be needed anymore
	void unloadTexture(std::string path);

	// unload specific font; useful when you know which file won't be needed anymore
	void unloadFont(std::string path);

	// unload specific savedata; useful when you know which file won't be needed anymore
	void unloadSavedata(std::string path);
	
	// get progress
	float calcProgress() const;

	// singleton design
	static ResourceManager* GetResourceManager();
	static void CleanUp();

private:

	ResourceLoader resourceLoader;

	std::map<std::string, sf::Texture*> allTextures;
	std::map<std::string, sf::SoundBuffer*> allAudio;
	std::map<std::string, sf::Font*> allFont;
	std::map<std::string, SavedataReader*> allSavedata;

	// default constructor/destructor
	ResourceManager();
	~ResourceManager();

	static ResourceManager* m_pThis;
};
