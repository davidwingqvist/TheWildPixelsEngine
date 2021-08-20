#include "RenderAble.h"

Renderable::Renderable()
{
	this->doRender = true;
	this->clickAble = false;
}

Renderable::~Renderable()
{
}

void Renderable::ToggleRender(const bool&& toggle)
{
	this->doRender = toggle;
}

void Renderable::ToggleClickAble(const bool&& toggle)
{
}

const bool Renderable::IsRender() const
{
	return this->doRender;
}

const bool Renderable::IsClickAble() const
{
	return this->clickAble;
}
