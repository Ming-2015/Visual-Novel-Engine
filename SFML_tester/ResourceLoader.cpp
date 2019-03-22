#include "ResourceLoader.h"

ResourceLoader::ResourceLoader()
{

}

ResourceLoader::~ResourceLoader()
{
}

void ResourceLoader::addTexture(sf::Texture* tex, std::string path)
{
	textureLoaders.push_back(TextureLoaderThread(path.c_str(), tex));
}

void ResourceLoader::addAudio(sf::SoundBuffer* audio, std::string path)
{
	audioLoaders.push_back(AudioLoaderThread(path, audio));
}

bool ResourceLoader::reset()
{
	if (!started)
	{
		textureLoaders.clear();
		audioLoaders.clear();
		allThreads.clear();
		return true;
	}
	else
	{
		if (doneLoading())
		{
			textureLoaders.clear();
			audioLoaders.clear();
			allThreads.clear();
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

	for (TextureLoaderThread& t : textureLoaders)
	{
		if (!t.hasStartedLoading())
		{
			allThreads[static_cast<void*>(t.texPtr)] = std::thread([&t] {t.startLoading(); });
		}
	}

	for (AudioLoaderThread& t : audioLoaders)
	{
		if (!t.hasStartedLoading())
		{
			allThreads[static_cast<void*>(t.audioPtr)] = std::thread([&t] {t.startLoading(); });
		}
	}
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
		for (const TextureLoaderThread& t : textureLoaders)
		{
			if (!t.isDone())
			{
				done = false;
				break;
			}
		}

		for (const AudioLoaderThread& t : audioLoaders)
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
	textureLoaders.clear();
	audioLoaders.clear();
}

void ResourceLoader::join(sf::SoundBuffer * ptr)
{
	join(static_cast<void*>(ptr));
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
