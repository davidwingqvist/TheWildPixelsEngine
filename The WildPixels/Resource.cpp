#include "Resource.h"

Resource::Resource(ResourceType &&type)
{
    this->type = type;
}

const ResourceType Resource::GetType() const
{
    return ResourceType();
}
