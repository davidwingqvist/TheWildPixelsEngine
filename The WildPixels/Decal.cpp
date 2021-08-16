#include "Decal.h"
#include "stb_image.h"
#include "Graphics.h"

bool Decal::CreateTexture(const std::string file_path)
{
	HRESULT hr;
	int width, height, channels;
	unsigned char* image = stbi_load(file_path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	if (width <= 0 || height <= 0)
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC tDesc;
	tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.ArraySize = 1;
	tDesc.MipLevels = 1;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.CPUAccessFlags = 0;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tDesc.MiscFlags = 0;
	tDesc.Width = width;
	tDesc.Height = height;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = (void*)image;
	data.SysMemPitch = static_cast<UINT>(width * 4);
	data.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = DEVICE->CreateTexture2D(&tDesc, &data, &this->decalTexture);
	if (SUCCEEDED(hr))
		DEVICE->CreateShaderResourceView(this->decalTexture, 0, &this->decalShaderView);
	else
	{
		stbi_image_free(image);
		return false;
	}

	stbi_image_free(image);
	return true;
}

bool Decal::CreateVertexBuffer(float* x, float* y)
{
	float x_pos = (*x) * 0.01f;
	float y_pos = (*y) * 0.01f;

	ScreenVertex screenQuad[4] =
	{
		{ { -x_pos, -y_pos, 0.0f }, { 0.0f, 1.0f } }, // BOTTOM LEFT
		{ { -x_pos, y_pos, 0.0f }, { 0.0f, 0.0f } },   // TOP LEFT
		{ { x_pos, y_pos, 0.0f }, { 1.0f, 0.0f } },   // TOP RIGHT
		{ { x_pos, -y_pos, 0.0f }, { 1.0f, 1.0f } }    // BOTTOM RIGHT
	};

	UINT indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	HRESULT hr;
	D3D11_BUFFER_DESC bDesc;
	bDesc.ByteWidth = (UINT)4 * sizeof(ScreenVertex);
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &(screenQuad[0]);

	hr = DEVICE->CreateBuffer(&bDesc, &data, &this->vertexBuffer);
	if (FAILED(hr))
		return false;

	bDesc.ByteWidth = sizeof(UINT) * 6;
	bDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bDesc.CPUAccessFlags = 0;
	bDesc.MiscFlags = 0;

	data.pSysMem = &(indices[0]);
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	hr = DEVICE->CreateBuffer(&bDesc, &data, &this->indexBuffer);
	if (FAILED(hr))
		return false;

	return true;
}

bool Decal::CreateVertexBuffer(float* x, float* y, float width, float height)
{
	return false;
}

Decal::Decal(const std::string&& decal_path)
{
	this->decalShaderView = nullptr;
	this->decalTexture = nullptr;
	this->vertexBuffer = nullptr;
	this->doRender = true;

	this->CreateTexture(decal_path);
	float y = (float)Graphics::GetHeight() * 0.5f;
	float x = (float)Graphics::GetWidth() * 0.5f;
	this->CreateVertexBuffer(&x, &y);
	this->position.x = x;
	this->position.y = y;

}

Decal::Decal(const std::string&& decal_path, float x, float y)
{
	this->decalShaderView = nullptr;
	this->decalTexture = nullptr;
	this->vertexBuffer = nullptr;
	this->doRender = true;

	this->position.x = x;
	this->position.y = y;

	this->CreateTexture(decal_path);
	this->CreateVertexBuffer(&x, &y);
}

Decal::Decal(const std::string&& decal_path, Vector2D& pos)
{
	this->decalShaderView = nullptr;
	this->decalTexture = nullptr;
	this->vertexBuffer = nullptr;
	this->doRender = true;

	this->position = pos;

	this->CreateTexture(decal_path);
	this->CreateVertexBuffer(&pos.x, &pos.y);
}

Decal::Decal()
{
	this->decalShaderView = nullptr;
	this->decalTexture = nullptr;
	this->vertexBuffer = nullptr;
	this->doRender = true;

	this->CreateTexture("Textures/Theunit.png");
	float y = (float)Graphics::GetHeight() * 0.5f;
	float x = (float)Graphics::GetWidth() * 0.5f;
	this->CreateVertexBuffer(&x, &y);
	this->position.x = x;
	this->position.y = y;
}

Decal::~Decal()
{
	if (this->decalTexture)
		this->decalTexture->Release();
	if (this->decalShaderView)
		this->decalShaderView->Release();
	if (this->vertexBuffer)
		this->vertexBuffer->Release();
}

void Decal::Resize(float width, float height)
{
}

void Decal::ToggleRender(const bool&& toggle)
{
	this->doRender = toggle;
}

const bool Decal::IsRender() const
{
	return this->doRender;
}

void Decal::Render()
{
}
