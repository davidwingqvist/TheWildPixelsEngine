#include "Graphics2D.h"
#include "Graphics.h"
#include <dwrite_1.h>
#include <dwrite_2.h>
#include <dwrite_3.h>
#include <cassert>


Graphics2D* Graphics2D::instance = nullptr;

#define INSTANCE Graphics2D::instance

//CreateDxgiSurfaceRenderTarget

// Documentation for 2D writing onto 3D application
// https://docs.microsoft.com/en-us/windows/win32/direct2d/direct2d-and-direct3d-interoperation-overview#supported-direct3d-versions

// Share textures
// https://stackoverflow.com/questions/4485265/cant-create-direct2d-dxgi-surface

Graphics2D::Graphics2D(UINT width, UINT height)
{
	HRESULT result, hr;
	this->winHeight = height;
	this->winWidth = width;

	UINT flags = D3D10_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG

	flags = D3D10_CREATE_DEVICE_DEBUG | D3D10_CREATE_DEVICE_BGRA_SUPPORT;

#endif

	
	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), 
		reinterpret_cast<IUnknown**>(&this->writeFactory));
	/*
		Create a factory for 2D object drawing,
		Also create a D3D10Device with the same adapter as D3D11 device.
	*/
	D2D1_FACTORY_OPTIONS options;

#ifdef _DEBUG
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
	options.debugLevel = 0;
#endif

	result = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, options, &this->factory);
	//device_result = D3D10CreateDevice(ADAPTER, D3D10_DRIVER_TYPE_HARDWARE, nullptr, flags, D3D10_SDK_VERSION, &this->device);
	// Factory was able to be made. Proceed.
	if (SUCCEEDED(result))
	{
		Graphics::GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&this->surface));
		FLOAT dpiX;
		FLOAT dpiY;
		dpiX = (FLOAT)GetDpiForWindow(Graphics::GetWindow());
		dpiY = dpiX;

		D2D1_RENDER_TARGET_PROPERTIES props =
			D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_DEFAULT,
				D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
				dpiX,
				dpiY
			);

		hr = this->factory->CreateDxgiSurfaceRenderTarget(this->surface, &props, &this->backBufferView);


		RECT rc;
		GetClientRect(Graphics::GetWindow(), &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
		hr = this->factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(Graphics::GetWindow(), 
				D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)), 
			&this->windowTarget);

		if (SUCCEEDED(hr))
		{
			this->backBufferView->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f), &this->defaultBrush);
		}

		hr = this->writeFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			50.0f,
			L"en-us",
			&this->defaultFormat
		);
		if (SUCCEEDED(hr))
		{
			hr = this->defaultFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		}
		if (SUCCEEDED(hr))
			hr = this->defaultFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
}

Graphics2D::~Graphics2D()
{
	if (this->factory)
		this->factory->Release();
	if (this->backBufferView)
		this->backBufferView->Release();
	if (this->surface)
		this->surface->Release();
	if (this->device)
		this->device->Release();
	if (this->windowTarget)
		this->windowTarget->Release();
	if (this->defaultBrush)
		this->defaultBrush->Release();
	if (this->defaultFormat)
		this->defaultFormat->Release();
}

bool Graphics2D::Initialize(UINT width, UINT height)
{
	if (!INSTANCE)
	{
		INSTANCE = new Graphics2D(width, height);
		if (INSTANCE)
			return true;
		else
			return false;
	}

	return true;
}

ID2D1Factory*& Graphics2D::GetFactory()
{
	return INSTANCE->factory;
}

ID3D10Device*& Graphics2D::GetDevice()
{
	return INSTANCE->device;
}

IDWriteTextFormat* Graphics2D::CreateTextFormat(float font_size)
{
	if (INSTANCE->writeFactory)
	{
		HRESULT hr;
		IDWriteTextFormat* format;
		hr = INSTANCE->writeFactory->CreateTextFormat(
			L"Times New Roman",
			NULL,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			font_size,
			L"en-us",
			&format
		);
		if (SUCCEEDED(hr))
		{
			hr = format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		}
		if (SUCCEEDED(hr))
			hr = format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		return format;
	}
	else
		return nullptr;
}

void Graphics2D::Draw(const std::string&& text, IDWriteTextFormat* format)
{

	if (INSTANCE->windowTarget)
	{
		IDWriteTextFormat* current_format = INSTANCE->defaultFormat;
		if (format)
			current_format = format;

		RECT rc;
		GetClientRect(Graphics::GetWindow(), &rc);
		D2D1_RECT_F layoutRect = D2D1::RectF(
			static_cast<FLOAT>(rc.left),
			static_cast<FLOAT>(rc.top),
			static_cast<FLOAT>(rc.right - rc.left),
			static_cast<FLOAT>(rc.bottom - rc.top)
		);

		const char* t = text.c_str();
		const WCHAR* pwcsName;
		int nChars = MultiByteToWideChar(CP_ACP, 0, t, -1, NULL, 0);
		pwcsName = new WCHAR[nChars];
		MultiByteToWideChar(CP_ACP, 0, t, -1, (LPWSTR)pwcsName, nChars);

		INSTANCE->backBufferView->BeginDraw();
		//INSTANCE->backBufferView->Clear(D2D1::ColorF(D2D1::ColorF::Black));
		INSTANCE->backBufferView->SetTransform(D2D1::IdentityMatrix());

		INSTANCE->backBufferView->DrawTextW(pwcsName,
			(UINT32)text.length(),
			current_format,
			layoutRect,
			INSTANCE->defaultBrush
		);

		HRESULT hr = INSTANCE->backBufferView->EndDraw();

		delete[] pwcsName;
	}
}

void Graphics2D::Draw(std::string&& text, float x, float y, IDWriteTextFormat* format)
{
}

void Graphics2D::Destroy()
{
	if (INSTANCE)
	{
		delete INSTANCE;
	}
}