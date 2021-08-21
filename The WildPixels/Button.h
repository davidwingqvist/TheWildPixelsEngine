#pragma once
#include "RenderAble.h"
#include "Texture.h"
#include <d3d11.h>

class Button : public Renderable
{
private:

	Texture* texture = nullptr;
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;

	bool activate = false;
public:

	Button();
	virtual ~Button() override;

	// Use this at the beginning of press action.
	void Press();

	// Use this to end the press action.
	void End();
	virtual void Render() override;
	virtual const bool Colliding(float* x, float* y) override;

};