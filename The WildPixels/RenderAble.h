#pragma once
/*
	Class for objects that will be renderable.
*/
class Renderable
{
private:

	bool doRender;

public:

	Renderable();
	virtual ~Renderable();

	void ToggleRender(const bool&& toggle);
	const bool IsRender() const;

	virtual void Render() = 0;
};