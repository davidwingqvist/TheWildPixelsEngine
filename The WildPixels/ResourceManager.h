#pragma once
#include <map>
#include <string>
#include "Resource.h"
#include "Texture.h"
#include "Mesh.h"

class ResourceManager
{
private:

	/*
		Thread compatible.
	*/
	size_t lockManager;
	size_t lockClearCache;

private:

	std::map<std::string, Resource*> resources;


public:

	static ResourceManager* instance;
	ResourceManager();
	~ResourceManager();
	static void Initialize();
	static void Destroy();
	static Resource* GetResource(std::string &&regName);
	static void InsertResource(Resource* resource, std::string &&regName);
	static void RemoveResource(std::string&& regName);
	static void ClearCache();
};

/*
	Get a resource from the resource manager, when failed a nullptr will be returned.
*/
#define RESOURCE(type, name) dynamic_cast<type*>(ResourceManager::instance->GetResource(name));

/*
	Add a resource to the resource manager.
*/
#define ADD_RESOURCE(resource, filePath) ResourceManager::InsertResource(resource, std::string(filePath));