#pragma once
#include <d3d11.h>
#include "Window.h"
#include <iostream>

#define DEVICE Graphics::GetDevice()
#define CONTEXT Graphics::GetContext()

class Graphics
{
private:

	UINT winWidth;
	UINT winHeight;

private:
	Graphics();
	virtual ~Graphics();

	static Graphics* instance;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* backBufferView;
	IDXGISwapChain* swapChain;
	Window window;
	bool setup_device;
	bool setup_bBuffer;

	void SetUpDevice();
	void SetUpBackBuffer();

public:

	static float deltaTime;
	static bool Initialize(HINSTANCE instance, UINT width, UINT height);
	static void Destroy();
	static ID3D11Device*& GetDevice();
	static ID3D11DeviceContext*& GetContext();
	static void ClearScreen();
	static void Present();
	static UINT GetWidth();
	static UINT GetHeight();

	// No depth buffer.
	static void SetBackbufferAsTarget();
};