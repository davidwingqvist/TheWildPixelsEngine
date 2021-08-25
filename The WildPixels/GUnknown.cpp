#include "GUnknown.h"
GUnkown::GUnkown()
{

}

CALL GUnkown::Get(GUnkown** pointer)
{
	if (this == nullptr)
		return false;

	*pointer = this;
	return true;
}
