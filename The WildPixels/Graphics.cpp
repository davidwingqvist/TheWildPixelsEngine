#include "Graphics.h"
#include "multi_thread_manager.h"

#define GRAPHICS Graphics::instance

Graphics* Graphics::instance = nullptr;
float Graphics::deltaTime = 0.f;

Graphics::Graphics()
{
	this->device = nullptr;
	this->context = nullptr;
	this->swapChain = nullptr;
	this->backBufferView = nullptr;
	this->winHeight = 0;
	this->winWidth = 0;
	this->setup_device = true;
	this->setup_bBuffer = true;
}

Graphics::~Graphics()
{

}

void Graphics::SetUpDevice()
{
	HRESULT hr;

	// Debug mode
	UINT flags = 0;

#ifdef _DEBUG
		flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Change this feature level to change DirectX version, Most stable is DirectX11
	D3D_FEATURE_LEVEL directXfeature[] = { D3D_FEATURE_LEVEL_11_0 };
	DXGI_SWAP_CHAIN_DESC swapDesc;

	swapDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferDesc.Width = this->winWidth;
	swapDesc.BufferDesc.Height = this->winHeight;

	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	// One front buffer and one Back
	swapDesc.BufferCount = 2;

	// Allow our backbuffer to be manipulated by any compute shader, e.g. allows simple blur effect.
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = 0;
	swapDesc.OutputWindow = this->window.getWindow();

	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, directXfeature, 1, D3D11_SDK_VERSION,
		&swapDesc, &this->swapChain, &this->device, nullptr, &this->context);
	this->setup_device = false;

	// Get the adapter that was created with the device.
	IDXGIDevice* pDXGIDevice;
	hr = this->device->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
	if (pDXGIDevice)
	{
		hr = pDXGIDevice->GetAdapter( &this->adapter );
		pDXGIDevice->Release();
	}
}

void Graphics::SetUpBackBuffer()
{
	while (this->setup_device);

	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		this->setup_bBuffer = false;
		return;
	}

	HRESULT hr;
	hr = DEVICE->CreateRenderTargetView(backBuffer, NULL, &this->backBufferView);
	backBuffer->Release();
	this->setup_bBuffer = false;
}

void Graphics::ClearScreen()
{
	float color[4];
	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;
	color[3] = 0.0f;

	CONTEXT->ClearRenderTargetView(GRAPHICS->backBufferView, color);
}

void Graphics::Present()
{
	GRAPHICS->swapChain->Present(0, 0);
}

UINT Graphics::GetWidth()
{
	return GRAPHICS->winWidth;
}

UINT Graphics::GetHeight()
{
	return GRAPHICS->winHeight;
}

HWND Graphics::GetWindow()
{
	return GRAPHICS->window.getWindow();
}

void Graphics::SetBackbufferAsTarget()
{
	if(Graphics::instance->context != nullptr)
		CONTEXT->OMSetRenderTargets(1, &GRAPHICS->backBufferView, nullptr);
}


bool Graphics::Initialize(HINSTANCE instance, UINT width, UINT height)
{
	if (!GRAPHICS)
	{
		GRAPHICS = new Graphics;
		GRAPHICS->setup_device = true;
		GRAPHICS->setup_bBuffer = true;
		GRAPHICS->winHeight = height;
		GRAPHICS->winWidth = width;
		GRAPHICS->window.SetUp(instance, width, height);
		GRAPHICS->SetUpDevice();
		GRAPHICS->SetUpBackBuffer();
		return true;
	}
	else
		return false;
}

void Graphics::Destroy()
{
	if (GRAPHICS->backBufferView)
		GRAPHICS->backBufferView->Release();
	if (GRAPHICS->swapChain)
		GRAPHICS->swapChain->Release();
	GRAPHICS->context->Flush();
	if (GRAPHICS->context)
		GRAPHICS->context->Release();
	if (GRAPHICS->adapter)
		GRAPHICS->adapter->Release();
	if (GRAPHICS->device)
		GRAPHICS->device->Release();

	if (GRAPHICS)
		delete GRAPHICS;
}

ID3D11Device*& Graphics::GetDevice()
{
	return GRAPHICS->device;
}


ID3D11DeviceContext*& Graphics::GetContext()
{
	return GRAPHICS->context;
}

IDXGIAdapter*& Graphics::GetAdapter()
{
	return GRAPHICS->adapter;
}

