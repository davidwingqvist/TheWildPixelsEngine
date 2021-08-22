#pragma once
#include "RenderAble.h"
#include "Texture.h"
#include <d3d11.h>
#include "Structures.h"

/*
	Create a square button presented in 2D on the screen.
*/
class Button : public Renderable
{
private:

	Texture* texture = nullptr;
	Texture* toggledTexture = nullptr;
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11Buffer* colorBuffer = nullptr;
	Vector2D position;
	Vector3D color = {1.0f, 1.0f, 1.0f};
	Vector3D toggledColor = { .0f, .0f, .0f };
	float width = 0.35f;
	float height = 0.25f;

	std::string text;

	// Set if the button should be a toggle.
	bool isToggle = false;

	// Check if the button is toggled/pressed.
	bool isToggledPressed = false;

	void UpdateBuffer(const bool&& useColor);

	bool CreateVertex();
public:

	Button();
	Button(float x, float y);
	virtual ~Button() override;
	// Set this specific button as a toggle on/off.
	void SetAsToggle();
	// Set this specific button as press once and done deal.
	void SetAsPress();

	// Register the button as pressed.
	void Toggle();

	void ApplyTexture(const std::string&& filePath);
	void ApplyColor(Vector3D& color);
	void ApplyColor(float red, float green, float blue);

	virtual void Render() override;
	virtual const bool Colliding(float* x, float* y) override;

	/*
		Functions to use when the button is set to toggled.
	*/

	// Set the texture to switch to when pressed during a toggled state.
	void ToggledTexture(const std::string&& filePath);

	/*
		Set the color when on a toggled state.
	*/
	void ToggledColor(Vector3D& color);
	void ToggledColor(float red, float green, float blue);

	void SetText(const std::string&& text);
};