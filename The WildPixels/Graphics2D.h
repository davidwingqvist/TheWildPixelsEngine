#pragma once
#include <d2d1.h>
#include <dwrite.h>
#include <string>


class Graphics2D
{
private:

	UINT winWidth;
	UINT winHeight;

private:

	static Graphics2D* instance;
	ID2D1Factory* factory = nullptr;
	IDWriteFactory* writeFactory = nullptr;
	ID3D10Device* device = nullptr;
	ID2D1RenderTarget* backBufferView = nullptr;
	IDXGISurface1* surface = nullptr;
	ID2D1HwndRenderTarget* windowTarget = nullptr;
	ID2D1SolidColorBrush* defaultBrush = nullptr;
	IDWriteTextFormat* defaultFormat = nullptr;
	Graphics2D(UINT width, UINT height);
	~Graphics2D();

public:

	static bool Initialize(UINT width, UINT height);
	static ID2D1Factory*& GetFactory();
	static ID3D10Device*& GetDevice();
	static IDWriteTextFormat* CreateTextFormat(float font_size);

	// Draw text in the middle of the screen.
	static void Draw(const std::string&& text, IDWriteTextFormat* format = nullptr);

	// Draw text at the specified x and y position on the screen.
	static void Draw(std::string&& text, float x, float y, IDWriteTextFormat* format = nullptr);
	static void Destroy();
};

#define FACTORY Graphics2D::GetFactory()
#define DEVICE2D Graphics2D::GetDevice()