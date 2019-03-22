#include "ResourceLoader.h"

ResourceLoader::ResourceLoader()
{
	LOGGER->Log("ResourceLoader", "ResouceLoader constructor called!");
}

ResourceLoader::~ResourceLoader()
{
}

void ResourceLoader::addTexture(sf::Texture* tex, std::string path)
{
	TextureLoaders.push_back(TextureLoaderThread(path.c_str(), tex));
}

void ResourceLoader::addAudio(sf::Music* audio, std::string path)
{
	AudioLoaders.push_back(AudioLoaderThread(path, audio));
}

bool ResourceLoader::reset()
{
	if (!started)
	{
		TextureLoaders.clear();
		AudioLoaders.clear();
		return true;
	}
	else
	{
		if (doneLoading())
		{
			TextureLoaders.clear();
			AudioLoaders.clear();
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
	if (!started)
	{
		started = true;

		for (TextureLoaderThread& t : TextureLoaders)
		{
			allThreads.push_back( std::thread([&t] {t.startLoading(); }) );
		}

		for (AudioLoaderThread& t : AudioLoaders)
		{
			allThreads.push_back( std::thread([&t] {t.startLoading(); }) );
		}
	}
	else
	{
		LOGGER->Log("ResourceLogger", "ResourceLoader already started!");
	}
}

bool ResourceLoader::doneLoading()
{
	if (!started)
	{
		return false;
	}
	else
	{
		bool done = true;
		for (TextureLoaderThread& t : TextureLoaders)
		{
			if (!t.isDone())
			{
				done = false;
				break;
			}
		}

		for (AudioLoaderThread& t : AudioLoaders)
		{
			if (!t.isDone())
			{
				done = false;
				break;
			}
		}

		return done;
	}
}

bool ResourceLoader::hasStarted()
{
	return started;
}

void ResourceLoader::join()
{
	for (std::thread& t : allThreads)
	{
		t.join();
	}

	started = false;
	allThreads.clear();
	TextureLoaders.clear();
	AudioLoaders.clear();
}
