#include "ResourceManager.h"

ResourceManager* ResourceManager::instance = nullptr;
#define MANAGER ResourceManager::instance

ResourceManager::ResourceManager()
{
	this->lockManager = 0;
	this->lockClearCache = 0;
}

ResourceManager::~ResourceManager()
{
	for (auto it = this->resources.begin(); it != this->resources.end(); it++)
	{
		if (it->second)
		{
			delete it->second;
		}
	}
	this->resources.clear();
}

void ResourceManager::Initialize()
{
	if (MANAGER == nullptr)
	{
		MANAGER = new ResourceManager();
	}
	else
		return;
}

void ResourceManager::Destroy()
{
	if (MANAGER != nullptr)
	{
		delete MANAGER;
	}
}

Resource* ResourceManager::GetResource(std::string&& regName)
{
	// Sync with threads.
	while (MANAGER->lockManager > 0);

	MANAGER->lockClearCache++;
	if ((int)MANAGER->resources.size() > 0)
	{
		auto rs = MANAGER->resources.find(regName);

		if (rs != MANAGER->resources.end())
		{
			MANAGER->lockClearCache = 0;
			return rs->second;
		}
		else
		{	// No resource found.
			MANAGER->lockClearCache = 0;
			return nullptr;
		}
	}
	else
	{	// Either no size or manager is currently locked.
		MANAGER->lockClearCache = 0;
		return nullptr;
	}
	MANAGER->lockClearCache = 0;
}

void ResourceManager::InsertResource(Resource* resource, std::string &&regName)
{
	// Sync with threads.
	while (MANAGER->lockManager > 0);
	MANAGER->lockClearCache++;
	if ((int)MANAGER->resources.size() >= 0)
	{
		auto rs = MANAGER->resources.find(regName);

		if (rs == MANAGER->resources.end())
		{
			MANAGER->resources.emplace(regName, resource);
			MANAGER->lockClearCache = 0;
		}
		else
		{
			delete rs->second;
			MANAGER->resources.erase(regName);
			MANAGER->resources.emplace(regName, resource);
			MANAGER->lockClearCache = 0;
		}
	}
	MANAGER->lockClearCache = 0;
}

void ResourceManager::RemoveResource(std::string&& regName)
{
	if ((int)MANAGER->resources.size() > 0)
	{
		auto rs = MANAGER->resources.find(regName);

		if (rs != MANAGER->resources.end())
		{
			delete rs->second;
			MANAGER->resources.erase(regName);
		}
	}
}

void ResourceManager::ClearCache()
{
	MANAGER->lockManager++;

	while (MANAGER->lockClearCache > 0);

	for (auto it = MANAGER->resources.begin(); it != MANAGER->resources.end(); it++)
	{
		if (it->second)
		{
			delete it->second;
		}
	}
	MANAGER->resources.clear();

	MANAGER->lockManager = 0;
}
