#include "Graphics2D.h"
#include "Graphics.h"

Graphics2D* Graphics2D::instance = nullptr;

#define INSTANCE Graphics2D::instance

//CreateDxgiSurfaceRenderTarget

// Documentation for 2D writing onto 3D application
// https://docs.microsoft.com/en-us/windows/win32/direct2d/direct2d-and-direct3d-interoperation-overview#supported-direct3d-versions

// Share textures
// https://stackoverflow.com/questions/4485265/cant-create-direct2d-dxgi-surface

Graphics2D::Graphics2D(UINT width, UINT height)
{
	HRESULT result, device_result;
	this->winHeight = height;
	this->winWidth = width;

	UINT flags = D3D10_CREATE_DEVICE_BGRA_SUPPORT;

	result = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &this->factory);
	device_result = D3D10CreateDevice(nullptr, D3D10_DRIVER_TYPE_HARDWARE, nullptr, flags, D3D10_SDK_VERSION, &this->device);
	// Factory was able to be made. Proceed.
	if (SUCCEEDED(result) && SUCCEEDED(device_result))
	{
		DXGI_SURFACE_DESC sDesc;
		sDesc.Height = height;
		sDesc.Width = width;
		sDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
		sDesc.SampleDesc.Count = 1;
		sDesc.SampleDesc.Quality = 0;

		D2D1_RENDER_TARGET_PROPERTIES rProp;
		rProp.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
		rProp.dpiX = 0;
		rProp.dpiY = 0;
		rProp.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;
		rProp.pixelFormat.format = DXGI_FORMAT_R32G32B32A32_UINT;
		rProp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
		rProp.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
		
		//this->factory->CreateDxgiSurfaceRenderTarget(this->surface, rProp, &this->renderTarget);
	}
}

Graphics2D::~Graphics2D()
{
	if (this->factory)
		this->factory->Release();
	if (this->renderTarget)
		this->renderTarget->Release();
	if (this->surface)
		this->surface->Release();
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

ID2D1Factory* Graphics2D::GetFactory()
{
	return this->factory;
}

void Graphics2D::Destroy()
{
	if (INSTANCE)
	{
		delete INSTANCE;
	}
}
