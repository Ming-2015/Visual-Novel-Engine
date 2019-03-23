#include "ResourceManager.h"

ResourceManager* ResourceManager::m_pThis = nullptr;

bool ResourceManager::addTexture(std::string path)
{
	if (allTextures.find(path) == allTextures.end())
	{
		allTextures[path] = nullptr;
		return true;
	}
	else return false;
}

bool ResourceManager::addAudio(std::string path)
{
	if (allAudio.find(path) == allAudio.end())
	{
		allAudio[path] = nullptr;
	}
	else return false;
}

bool ResourceManager::addFont(std::string path)
{
	if (allFont.find(path) == allFont.end())
	{
		allFont[path] = nullptr;
	}
	else return false;
}

bool ResourceManager::addSavedata(std::string path)
{
	if (allSavedata.find(path) == allSavedata.end())
	{
		allSavedata[path] = nullptr;
	}
	else return false;
}

void ResourceManager::startLoading()
{
	for (auto it : allTextures)
	{
		if (!it.second)
		{
			it.second = new sf::Texture();
			resourceLoader.addTexture(it.second, it.first);
		}
	}

	for (auto it : allAudio)
	{
		if (!it.second)
		{
			it.second = new sf::SoundBuffer();
			resourceLoader.addAudio(it.second, it.first);
		}
	}

	for (auto it : allFont)
	{
		if (!it.second)
		{
			it.second = new sf::Font();
			resourceLoader.addFont(it.second, it.first);
		}
	}

	for (auto it : allSavedata)
	{
		if (!it.second)
		{
			it.second = new SavedataReader();
			resourceLoader.addSavedata(it.second, it.first);
		}
	}

	resourceLoader.startAll();
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

	auto it_font = allFont.find(path);
	if (it_font != allFont.end())
	{
		resourceLoader.join((*it_font).second);
		return;
	}

	auto it_savedata = allSavedata.find(path);
	if (it_savedata != allSavedata.end())
	{
		resourceLoader.join((*it_savedata).second);
		return;
	}
}

void ResourceManager::joinTexture(std::string path)
{
	auto it_tex = allTextures.find(path);
	if (it_tex != allTextures.end() && (*it_tex).second)
	{
		resourceLoader.join((*it_tex).second);
	}
}

void ResourceManager::joinAudio(std::string path)
{
	auto it_audio = allAudio.find(path);
	if (it_audio != allAudio.end() && (*it_audio).second)
	{
		resourceLoader.join((*it_audio).second);
	}
}

void ResourceManager::joinFont(std::string path)
{
	auto it_font = allFont.find(path);
	if (it_font != allFont.end() && (*it_font).second)
	{
		resourceLoader.join((*it_font).second);
	}
}

void ResourceManager::joinSavedata(std::string path)
{
	auto it_savedata = allSavedata.find(path);
	if (it_savedata != allSavedata.end() && (*it_savedata).second)
	{
		resourceLoader.join((*it_savedata).second);
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
			resourceLoader.startTexture(allTextures[path]);
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
		resourceLoader.startTexture(allTextures[path]);
		return allTextures[path];
	}
}

sf::SoundBuffer* ResourceManager::getAudio(std::string path)
{
	// check if the resource is in the map
	auto it = allAudio.find(path);
	if (it != allAudio.end())
	{
		// if the audio is initialized
		if ((*it).second)
		{
			return (*it).second;
		}
		// otherwise, initialize it_audio forcefully
		else
		{
			allAudio[path] = new sf::SoundBuffer();
			resourceLoader.addAudio(allAudio[path], path);
			resourceLoader.startAudio(allAudio[path]);
			return allAudio[path];
		}
	}
	// if not, add it_audio then load;
	else
	{
		addAudio(path);
		allAudio[path] = new sf::SoundBuffer();
		resourceLoader.addAudio(allAudio[path], path);
		resourceLoader.startAudio(allAudio[path]);
		return allAudio[path];
	}
}

sf::Font * ResourceManager::getFont(std::string path)
{
	// check if the resource is in the map
	auto it = allFont.find(path);
	if (it != allFont.end())
	{
		// if the font is initialized
		if ((*it).second)
		{
			return (*it).second;
		}
		// otherwise, initialize font forcefully
		else
		{
			allFont[path] = new sf::Font();
			resourceLoader.addFont(allFont[path], path);
			resourceLoader.startFont(allFont[path]);
			return allFont[path];
		}
	}
	// if not, add font then load;
	else
	{
		addFont(path);
		allFont[path] = new sf::Font();
		resourceLoader.addFont(allFont[path], path);
		resourceLoader.startFont(allFont[path]);
		return allFont[path];
	}
}

SavedataReader * ResourceManager::getSavedata(std::string path)
{
	// check if the resource is in the map
	auto it = allSavedata.find(path);
	if (it != allSavedata.end())
	{
		// if the savedata is initialized
		if ((*it).second)
		{
			return (*it).second;
		}
		// otherwise, initialize savedata forcefully
		else
		{
			allSavedata[path] = new SavedataReader();
			resourceLoader.addSavedata(allSavedata[path], path);
			resourceLoader.startSavedata(allSavedata[path]);
			return allSavedata[path];
		}
	}
	// if not, add savedata then load;
	else
	{
		addSavedata(path);
		allSavedata[path] = new SavedataReader();
		resourceLoader.addSavedata(allSavedata[path], path);
		resourceLoader.startSavedata(allSavedata[path]);
		return allSavedata[path];
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

	for (auto it : allFont)
	{
		if (it.second)
		{
			delete it.second;
		}
	}
	allFont.clear();

	for (auto it : allSavedata)
	{
		if (it.second)
		{
			delete it.second;
		}
	}
	allSavedata.clear();
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

void ResourceManager::unloadFont(std::string path)
{
	auto it = allFont.find(path);
	if (it != allFont.end())
	{
		if ((*it).second)
		{
			delete (*it).second;
		}
		allFont.erase(it);
	}
}

void ResourceManager::unloadSavedata(std::string path)
{
	auto it = allSavedata.find(path);
	if (it != allSavedata.end())
	{
		if ((*it).second)
		{
			delete (*it).second;
		}
		allSavedata.erase(it);
	}
}

float ResourceManager::calcProgress() const
{
	return resourceLoader.calcProgress();
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
