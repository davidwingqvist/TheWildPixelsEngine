#pragma once
#include <d3d11.h>

enum class ResourceType {
	TEXTURE,
	MESH,
	AUDIO,
};

class Resource
{
private:

	ResourceType type;

public:

	Resource(ResourceType &&type);
	virtual ~Resource() = 0 {};

	const ResourceType GetType() const;
};