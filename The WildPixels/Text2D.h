#pragma once
#include "RenderAble.h"
#include "Structures.h"
#include <string>

class Text2D : public Renderable
{
private:

	Vector2D position = {0.0f, 0.0f};
	std::string text = "";
	bool hasPosition = false;

public:

	Text2D(const std::string&& text);
	Text2D(const std::string& text);

	void SetPosition(float x, float y);
	void SetText(const std::string&& text);
	void SetText(const std::string& text);

	virtual void Render() override;

	virtual const bool Colliding(float* x, float* y) override;

};