#include "RenderAble.h"

Renderable::Renderable()
{
	this->doRender = true;
}

Renderable::~Renderable()
{
}

void Renderable::ToggleRender(const bool&& toggle)
{
	this->doRender = toggle;
}

const bool Renderable::IsRender() const
{
	return this->doRender;
}
