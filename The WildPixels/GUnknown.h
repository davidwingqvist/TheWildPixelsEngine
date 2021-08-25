#pragma once
#include "Defines.h"

class GUnkown
{
public:

	GUnkown();
	virtual ~GUnkown() = 0 {};

	// Make the pointer point to this object.
	CALL Get(GUnkown** pointer);

	// Get resource.
	template<class T>
	CALL GetResource(void** resource);
};

template<class T>
inline CALL GUnkown::GetResource(void** resource)
{
	return CALL();
}
