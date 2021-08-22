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
	ID2D1RenderTarget* renderTarget = nullptr;
	IDXGISurface* surface = nullptr;
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
	static void Draw(const std::string&& text, IDWriteTextFormat* format = nullptr);
	static void Destroy();
	static void Clear();
};

#define FACTORY Graphics2D::GetFactory()
#define DEVICE2D Graphics2D::GetDevice()