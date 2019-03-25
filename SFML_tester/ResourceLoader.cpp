#include "ResourceLoader.h"

// for creating thread to load textures
class TextureLoaderThread : public LoaderThread {
public:

	TextureLoaderThread(const std::string& path, sf::Texture* texPtr)
		:path(path), texPtr(texPtr) {}

	std::string getPath()
	{
		return path;
	}

	void* getPtr()
	{
		return static_cast<void*>(texPtr);
	}

	std::string path;
	sf::Texture* texPtr;

protected:
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
	}
};

// For creating thread to load small audio files
// For loading BGM that needs a lot of space, use sf::Music to stream
class AudioLoaderThread : public LoaderThread {
public:

	AudioLoaderThread(const std::string& path, sf::SoundBuffer* audioPtr)
		:path(path), audioPtr(audioPtr) {}

	std::string getPath()
	{
		return path;
	}

	void* getPtr()
	{
		return static_cast<void*>(audioPtr);
	}

	std::string path;
	sf::SoundBuffer* audioPtr;

protected:

	void startLoading()
	{
		if (!audioPtr->loadFromFile(path))
		{
			std::string err = "Failed to load audio: " + path;
			LOGGER->Log("AudioLoaderThread", err);
		}
		else
		{
			valid = true;
		}
	}
};

// For creating thread to load font files
class FontLoaderThread : public LoaderThread {
public:

	FontLoaderThread(const std::string& path, sf::Font* fontPtr)
		:path(path), fontPtr(fontPtr) {}

	std::string getPath()
	{
		return path;
	}

	void* getPtr()
	{
		return static_cast<void*>(fontPtr);
	}

	std::string path;
	sf::Font* fontPtr;

protected:

	void startLoading()
	{
		if (!fontPtr->loadFromFile(path))
		{
			std::string err = "Failed to load font: " + path;
			LOGGER->Log("FontLoaderThread", err);
		}
		else
		{
			valid = true;
		}
	}
};

// For creating thread to load savedata
class SavedataLoaderThread : public LoaderThread {
public:

	SavedataLoaderThread(const std::string& path, SavedataReader* fontPtr)
		:path(path), savedateReader(fontPtr) {}

	std::string getPath()
	{
		return path;
	}

	void* getPtr()
	{
		return static_cast<void*>(savedateReader);
	}

	std::string path;
	SavedataReader* savedateReader;

private:

	void startLoading()
	{
		if (!savedateReader->loadFromFile(path))
		{
			std::string err = "Failed to load savedata: " + path;
			LOGGER->Log("FontLoaderThread", err);
		}
		else
		{
			valid = true;
		}
	}
};

ResourceLoader::ResourceLoader()
{

}

ResourceLoader::~ResourceLoader()
{
	// delete the loaders
	for (auto t : allLoaders)
	{
		if (t.second) delete t.second;
	}
}

bool ResourceLoader::addTexture(sf::Texture* tex, std::string path, bool lowPriority)
{
	void* ptr = static_cast<void*> (tex);

	if (allLoaders.find(ptr) == allLoaders.end())
	{
		allLoaders[ptr] = new TextureLoaderThread(path, tex);
		if (!lowPriority) allLoaders[ptr]->prioritize();
		queuedLoaders[ptr] = allLoaders[ptr];
		return true;
	}
	else return false;
}

bool ResourceLoader::addAudio(sf::SoundBuffer* audio, std::string path, bool lowPriority)
{
	void* ptr = static_cast<void*> (audio);

	if (allLoaders.find(ptr) == allLoaders.end())
	{
		allLoaders[ptr] = new AudioLoaderThread(path, audio);
		if (!lowPriority) allLoaders[ptr]->prioritize();
		queuedLoaders[ptr] = allLoaders[ptr];
		return true;
	}
	else return false;
}

bool ResourceLoader::addFont(sf::Font * font, std::string path, bool lowPriority)
{
	void* ptr = static_cast<void*> (font);

	if (allLoaders.find(ptr) == allLoaders.end())
	{
		allLoaders[ptr] = new FontLoaderThread(path, font);
		if (!lowPriority) allLoaders[ptr]->prioritize();
		queuedLoaders[ptr] = allLoaders[ptr];
		return true;
	}
	else return false;
}

bool ResourceLoader::addSavedata(SavedataReader * savedata, std::string path, bool lowPriority)
{
	void* ptr = static_cast<void*> (savedata);

	if (allLoaders.find(ptr) == allLoaders.end())
	{
		allLoaders[ptr] = new SavedataLoaderThread(path, savedata);
		if (!lowPriority) allLoaders[ptr]->prioritize();
		queuedLoaders[ptr] = allLoaders[ptr];
		return true;
	}
	else return false;
}

bool ResourceLoader::reset()
{
	if (!started)
	{
		queuedLoaders.clear();
		allThreads.clear();

		for (auto t : allLoaders)
		{
			if (t.second) delete t.second;
		}
		allLoaders.clear();

		return true;
	}
	else
	{
		if (doneLoading())
		{
			queuedLoaders.clear();
			allThreads.clear();

			for (auto t : allLoaders)
			{
				if (t.second) delete t.second;
			}
			allLoaders.clear();

			started = false;
			return true;
		}
		else
		{
			return false;
		}
	}
}

void ResourceLoader::startAll()
{
	started = true;
	reset();

	for (auto it = queuedLoaders.begin(); it != queuedLoaders.end(); it++)
	{
		if (!(*it).second->hasStartedLoading())
		{
			LoaderThread* t = (*it).second;
			allThreads[static_cast<void*>(t->getPtr())] 
				= std::thread([t] {t->launch(); });
		}
	}
	queuedLoaders.clear();
}

bool ResourceLoader::startTexture(sf::Texture * tex)
{
	return start(static_cast<void*>(tex));
}

bool ResourceLoader::startAudio(sf::SoundBuffer * audio)
{
	return start(static_cast<void*>(audio));
}

bool ResourceLoader::startFont(sf::Font * font)
{
	return start(static_cast<void*>(font));
}

bool ResourceLoader::startSavedata(SavedataReader * savedata)
{
	return start(static_cast<void*>(savedata));
}

bool ResourceLoader::doneLoading() const
{
	if (!started)
	{
		return true;
	}
	else
	{
		bool done = true;
		for (auto t : allLoaders)
		{
			if (!t.second->isDone())
			{
				done = false;
				break;
			}
		}

		return done;
	}
}

bool ResourceLoader::hasStarted() const
{
	return started;
}

void ResourceLoader::joinAll()
{
	for (auto it = allThreads.begin(); it != allThreads.end(); ++it)
	{
		(*it).second.join();

		// check if its in the loaders
		auto it_loader = allLoaders.find((*it).first);
		if (it_loader != allLoaders.end()) 
		{
			if ((*it_loader).second) delete (*it_loader).second;
			allLoaders.erase(it_loader);
		}
	}
	allThreads.clear();
	
	started = false;
}

void ResourceLoader::join(sf::SoundBuffer * ptr)
{
	join(static_cast<void*>(ptr));
}

void ResourceLoader::join(sf::Font * ptr)
{
	join(static_cast<void*>(ptr));
}

void ResourceLoader::join(SavedataReader * ptr)
{
	join(static_cast<void*>(ptr));
}

void ResourceLoader::prioritize(sf::Texture * ptr)
{
	prioritize(static_cast<void*>(ptr));
}

void ResourceLoader::prioritize(sf::SoundBuffer * ptr)
{
	prioritize(static_cast<void*>(ptr));
}

void ResourceLoader::prioritize(sf::Font * ptr)
{
	prioritize(static_cast<void*>(ptr));
}

void ResourceLoader::prioritize(SavedataReader * ptr)
{
	prioritize(static_cast<void*>(ptr));
}

void ResourceLoader::join(sf::Texture * ptr)
{
	join(static_cast<void*>(ptr));
}

float ResourceLoader::calcProgress() const
{
	float countThreads = 0;
	float countDone = 0;
	for (auto it : allLoaders)
	{
		if (it.second->hasStartedLoading())
		{
			countThreads++;
			if (it.second->isDone())
			{
				countDone++;
			}
		}
	}

	if (countThreads <= 0) return 1.f;

	return countDone / countThreads;
}


void ResourceLoader::join(void* ptr)
{
	auto it = allThreads.find(ptr);
	if (it != allThreads.end())
	{
		auto it_loader = allLoaders.find(ptr);

		// prioritize the thread
		if (it_loader != allLoaders.end())
		{
			if ((*it_loader).second)
			{
				(*it_loader).second->prioritize();
			}
		}

		// wait for it to join
		(*it).second.join();

		// cleanup
		allThreads.erase(it);
		if (it_loader != allLoaders.end())
		{
			if ((*it_loader).second) delete (*it_loader).second;
			allLoaders.erase(it_loader);
		}

		if (allThreads.size() <= 0) started = false;
	}
}

bool ResourceLoader::start(void * ptr)
{
	auto it = queuedLoaders.find(ptr);
	if (it != queuedLoaders.end())
	{
		if (!(*it).second->hasStartedLoading())
		{
			LoaderThread* t = (*it).second;
			allThreads[static_cast<void*>(t->getPtr())]
				= std::thread([t] {t->launch(); });
			started = true;
		}
		queuedLoaders.erase(it);
		return true;
	}
	return false;
}

bool ResourceLoader::prioritize(void * ptr)
{
	auto it = allLoaders.find(ptr);
	if (it != allLoaders.end())
	{
		if ((*it).second->isDone()) return false;

		(*it).second->prioritize();
		return true;
	}
	else return false;
}
