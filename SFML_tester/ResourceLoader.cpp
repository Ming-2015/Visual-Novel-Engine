#include "ResourceLoader.h"

// for creating thread to load textures
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
};

// For creating thread to load small audio files
// For loading BGM that needs a lot of space, use sf::Music to stream
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

bool ResourceLoader::addTexture(sf::Texture* tex, std::string path)
{
	void* ptr = static_cast<void*> (tex);

	if (allLoaders.find(ptr) == allLoaders.end())
	{
		allLoaders[ptr] = new TextureLoaderThread(path, tex);
		queuedLoaders.push_back(allLoaders[ptr]);
		return true;
	}
	else return false;
}

bool ResourceLoader::addAudio(sf::SoundBuffer* audio, std::string path)
{
	void* ptr = static_cast<void*> (audio);

	if (allLoaders.find(ptr) == allLoaders.end())
	{
		allLoaders[ptr] = new AudioLoaderThread(path, audio);
		queuedLoaders.push_back(allLoaders[ptr]);
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

void ResourceLoader::start()
{
	started = true;
	reset();

	for (int i = 0; i < queuedLoaders.size(); i++)
	{
		if (!queuedLoaders[i]->hasStartedLoading())
		{
			LoaderThread* t = queuedLoaders[i];
			allThreads[static_cast<void*>(t->getPtr())] 
				= std::thread([t] {t->startLoading(); });
		}
	}
	queuedLoaders.clear();
}

bool ResourceLoader::doneLoading() const
{
	if (!started)
	{
		return false;
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
	}

	started = false;
	allThreads.clear();

	for (auto t : allLoaders)
	{
		if (t.second) delete t.second;
	}
	allLoaders.clear();
}

void ResourceLoader::join(sf::SoundBuffer * ptr)
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
	return countDone / countThreads;
}

void ResourceLoader::join(sf::Texture * ptr)
{
	join(static_cast<void*>(ptr));
}

void ResourceLoader::join(void* ptr)
{
	auto it = allThreads.find(ptr);
	if (it != allThreads.end())
	{
		(*it).second.join();
	}
}
