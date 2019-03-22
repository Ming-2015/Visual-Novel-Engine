#include "ResourceManager.h"

void ResourceManager::addTexture(std::string path)
{
	if (allTextures.find(path) == allTextures.end())
	{
		allTextures[path] = nullptr;
	}
}

void ResourceManager::addAudio(std::string path)
{
	if (allAudio.find(path) == allAudio.end())
	{
		allAudio[path] = nullptr;
	}
}

void ResourceManager::startLoading()
{
	for (auto it : allTextures)
	{
		it.second = new sf::Texture();
		resourceLoader.addTexture(it.second, it.first);
	}

	for (auto it : allAudio)
	{
		it.second = new sf::SoundBuffer();
		resourceLoader.addAudio(it.second, it.first);
	}

	resourceLoader.start();
}

bool ResourceManager::clear()
{
	return resourceLoader.reset();
}

bool ResourceManager::hasStartedLoading() const
{
	return resourceLoader.hasStarted();
}

bool ResourceManager::doneLoading() const
{
	return resourceLoader.doneLoading();
}

void ResourceManager::joinAll()
{
	resourceLoader.joinAll();
}

void ResourceManager::join(std::string path)
{
	auto it_tex = allTextures.find(path);
	if (it_tex != allTextures.end())
	{
		resourceLoader.join((*it_tex).second);
		return;
	}
	
	auto it_audio = allAudio.find(path);
	if (it_audio != allAudio.end())
	{
		resourceLoader.join((*it_audio).second);
		return;
	}
}

void ResourceManager::joinAudio(std::string path)
{
	auto it_tex = allTextures.find(path);
	if (it_tex != allTextures.end())
	{
		resourceLoader.join((*it_tex).second);
	}
}

void ResourceManager::joinTexture(std::string path)
{
	auto it_audio = allAudio.find(path);
	if (it_audio != allAudio.end())
	{
		resourceLoader.join((*it_audio).second);
	}
}

sf::Texture* ResourceManager::getTexture(std::string path)
{
	// check if the resource is in the map
	auto it = allTextures.find(path);
	if (it != allTextures.end())
	{
		// if the texture is initialized
		if ((*it).second)
		{
			return (*it).second;
		}
		// otherwise, initialize it_tex forcefully
		else
		{
			// create the new resource
			allTextures[path] = new sf::Texture();
			resourceLoader.addTexture(allTextures[path], path);
			resourceLoader.start();
			return allTextures[path];
		}
	}
	// if not, add it_tex then load;
	else
	{
		// add and create the new resource
		addTexture(path);
		allTextures[path] = new sf::Texture();
		resourceLoader.addTexture(allTextures[path], path);
		resourceLoader.start();
		return allTextures[path];
	}
}

sf::SoundBuffer* ResourceManager::getAudio(std::string path)
{
	// check if the resource is in the map
	auto it = allAudio.find(path);
	if (it != allAudio.end())
	{
		// if the texture is initialized
		if ((*it).second)
		{
			return (*it).second;
		}
		// otherwise, initialize it_tex forcefully
		else
		{
			startLoading();
			return (*it).second;
		}
	}
	// if not, add it_tex then load;
	else
	{
		addAudio(path);
		allTextures[path] = new sf::Texture();
		resourceLoader.addAudio(allAudio[path], path);
		resourceLoader.start();
		return allAudio[path];
	}
}

void ResourceManager::unloadAll()
{
	for (auto it : allTextures)
	{
		if (it.second)
		{
			delete it.second;
		}
	}
	allTextures.clear();

	for (auto it : allAudio)
	{
		if (it.second)
		{
			delete it.second;
		}
	}
	allAudio.clear();
}

void ResourceManager::unloadAudio(std::string path)
{
	auto it = allAudio.find(path);
	if (it != allAudio.end())
	{
		if ((*it).second)
		{
			delete (*it).second;
		}
		allAudio.erase(it);
	}
}

void ResourceManager::unloadTexture(std::string path)
{
	auto it = allTextures.find(path);
	if (it != allTextures.end())
	{
		if ((*it).second)
		{
			delete (*it).second;
		}
		allTextures.erase(it);
	}
}

ResourceManager * ResourceManager::GetResourceManager()
{
	if (!m_pThis)
		m_pThis = new ResourceManager();
	return m_pThis;
}

void ResourceManager::CleanUp()
{
	if (m_pThis)
		delete m_pThis;
}

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	unloadAll();
}
