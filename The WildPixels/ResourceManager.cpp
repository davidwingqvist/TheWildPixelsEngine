#include "ResourceManager.h"
#include <mutex>

std::mutex manager_mtx;

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
	if ((int)MANAGER->resources.size() > 0)
	{
		manager_mtx.lock();
		auto rs = MANAGER->resources.find(regName);
		if (rs != MANAGER->resources.end())
		{
			manager_mtx.unlock();
			return rs->second;
		}
		else
		{
			manager_mtx.unlock();
			return nullptr;
		}
	}
	else
	{
		return nullptr;
	}
}

void ResourceManager::InsertResource(Resource* resource, std::string &&regName)
{
	if ((int)MANAGER->resources.size() >= 0)
	{
		manager_mtx.lock();
		auto rs = MANAGER->resources.find(regName);

		if (rs == MANAGER->resources.end())
		{
			MANAGER->resources.emplace(regName, resource);
		}
		else
		{
			delete rs->second;
			MANAGER->resources.erase(regName);
			MANAGER->resources.emplace(regName, resource);
		}
		manager_mtx.unlock();
	}
}

void ResourceManager::RemoveResource(std::string&& regName)
{
	if ((int)MANAGER->resources.size() > 0)
	{
		manager_mtx.lock();
		auto rs = MANAGER->resources.find(regName);

		if (rs != MANAGER->resources.end())
		{
			delete rs->second;
			MANAGER->resources.erase(regName);
		}
		manager_mtx.unlock();
	}
}

void ResourceManager::ClearCache()
{
	manager_mtx.lock();
	for (auto it = MANAGER->resources.begin(); it != MANAGER->resources.end(); it++)
	{
		if (it->second)
		{
			delete it->second;
		}
	}
	MANAGER->resources.clear();
	manager_mtx.unlock();
}
