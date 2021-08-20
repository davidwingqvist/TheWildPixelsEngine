#pragma once
#include "Camera.h"
/*
	Class for objects that will be renderable.
*/
class Renderable
{
private:

	bool doRender;
	bool clickAble;

public:

	Renderable();
	virtual ~Renderable();

	void ToggleRender(const bool&& toggle);
	void ToggleClickAble(const bool&& toggle);
	const bool IsRender() const;
	const bool IsClickAble() const;

	virtual void Render(CamParts& camParts) = 0;
	virtual const bool Colliding(float* x, float* y) = 0;
};