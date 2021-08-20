#pragma once
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

	virtual const bool Colliding(float* x, float* y) = 0;
	virtual void Render() = 0;
};