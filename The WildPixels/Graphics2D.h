#pragma once
#include <d2d1.h>

class Graphics2D
{
private:

	UINT winWidth;
	UINT winHeight;

private:

	static Graphics2D* instance;
	ID2D1Factory* factory = nullptr;
	ID3D10Device* device = nullptr;
	ID2D1RenderTarget* renderTarget = nullptr;
	IDXGISurface* surface = nullptr;
	Graphics2D(UINT width, UINT height);
	~Graphics2D();

public:

	static bool Initialize(UINT width, UINT height);
	static ID2D1Factory*& GetFactory();
	static ID3D10Device*& GetDevice();
	static void Destroy();
};

#define FACTORY Graphics2D::GetFactory()
#define DEVICE2D Graphics2D::GetDevice()