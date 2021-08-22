#pragma once
#include "RenderAble.h"
#include "Structures.h"
#include "Texture.h"
#include "Decal.h"
#include "Button.h"
#include <d3d11.h>

constexpr int decal_amount = 10;
constexpr int button_amount = 5;

/*
	Create a canvas that will be presented in 2D on the screen.
	A canvas is able to be rendered in the background allowing other objects
	such as buttons, decals and more to be rendered on it.
*/
class Canvas : public Renderable
{
private:

	Texture* texture;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* colorBuffer;
	ID3D11BlendState* blendState;
	Vector3D color;
	Vector2D position;
	Vector2D size;

	Decal* decals[decal_amount];
	Button* buttons[button_amount];

	float transparency = 1.0f;

	bool CreateVertex();
	bool CreateTexture(const std::string& filePath);
	void UpdateBuffer(const bool&& useColor);

	void Unbind();

public:

	Canvas(const float&& x, const float&& y, const float&& width, const float height, const float&& red, const float&& blue, const float&& green);
	Canvas(Vector2D& position, Vector2D& size, Vector3D& color);
	virtual ~Canvas() override;
	virtual void Render() override;
	virtual const bool Colliding(float* x, float* y) override;
	const bool ApplyTexture(std::string&& filePath);
	void ApplyTransparency(float degree = .5f);
	void ApplyColor(Vector3D& color);
	void ApplyColor(float red, float green, float blue);

	// Apply a decal to this canvas, making this decal guaranteed visible on it. Max number of decals is defined by [decal_amount]
	void ApplyDecal(Decal* decal, int pos);
	// Apply a button onto this canvas.
	void ApplyButton(Button* button, int pos);
	void RePosition(float* x, float* y);
	void ReSize(float* width, float* height);
};