#include "Decal.h"
#include "stb_image.h"
#include "Graphics.h"

void Decal::CreateTexture(const std::string file_path)
{
	this->texture = RESOURCE(Texture, std::string(file_path));
	if (!this->texture)
	{
		this->texture = new Texture(std::string(file_path));
		ADD_RESOURCE(this->texture, file_path);
	}
}

bool Decal::CreateVertexBuffer(float* x, float* y)
{
	float x_pos = (*x);
	float y_pos = (*y);

	this->width = 0.25f;
	this->height = 0.25f;

	ScreenVertex screenQuad[4] =
	{
		{ { x_pos, y_pos - (float)this->height, 0.0f }, { 0.0f, 1.0f } }, // BOTTOM LEFT
		{ { x_pos, y_pos, 0.0f }, { 0.0f, 0.0f } },   // TOP LEFT
		{ { x_pos + (float)this->width, y_pos, 0.0f }, { 1.0f, 0.0f } },   // TOP RIGHT
		{ { x_pos + (float)this->width, y_pos - (float)this->height, 0.0f }, { 1.0f, 1.0f } }    // BOTTOM RIGHT
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
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
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
	float x_pos = (*x);
	float y_pos = (*y);

	this->width = width;
	this->height = height;

	ScreenVertex screenQuad[4] =
	{
		{ { x_pos, y_pos - (float)this->height, 0.0f }, { 0.0f, 1.0f } }, // BOTTOM LEFT
		{ { x_pos, y_pos, 0.0f }, { 0.0f, 0.0f } },   // TOP LEFT
		{ { x_pos + (float)this->width, y_pos, 0.0f }, { 1.0f, 0.0f } },   // TOP RIGHT
		{ { x_pos + (float)this->width, y_pos - (float)this->height, 0.0f }, { 1.0f, 1.0f } }    // BOTTOM RIGHT
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
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
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

void Decal::Unbind()
{
}

Decal::Decal(const std::string&& decal_path)
{
	this->vertexBuffer = nullptr;

	this->CreateTexture(decal_path);
	float y = (float)Graphics::GetHeight() * 0.01f;
	float x = (float)Graphics::GetWidth() * 0.01f;
	this->CreateVertexBuffer(&x, &y);
	this->position.x = x;
	this->position.y = y;

}

Decal::Decal(const std::string&& decal_path, float x, float y)
{
	this->vertexBuffer = nullptr;

	this->position.x = x;
	this->position.y = y;

	this->CreateTexture(decal_path);
	this->CreateVertexBuffer(&this->position.x, &this->position.y);
}

Decal::Decal(const std::string&& decal_path, Vector2D& pos)
{
	this->vertexBuffer = nullptr;
	this->position = pos;
	this->CreateTexture(decal_path);
	this->CreateVertexBuffer(&pos.x, &pos.y);
}

Decal::Decal(const std::string&& decal_path, float x, float y, float width, float height)
{
	this->vertexBuffer = nullptr;
	this->indexBuffer = nullptr;

	this->position.x = x;
	this->position.y = y;

	this->CreateTexture(decal_path);
	this->CreateVertexBuffer(&this->position.x, &this->position.y, width, height);
}

Decal::Decal()
{
	this->vertexBuffer = nullptr;

	this->CreateTexture("Textures/Theunit.png");
	float y = (float)Graphics::GetHeight() * 0.01f;
	float x = (float)Graphics::GetWidth() * 0.01f;
	this->CreateVertexBuffer(&x, &y);
	this->position.x = x;
	this->position.y = y;
}

Decal::~Decal()
{
	if (this->vertexBuffer)
		this->vertexBuffer->Release();
	if (this->indexBuffer)
		this->indexBuffer->Release();
}

void Decal::Resize(float width, float height)
{
	this->width = width;
	this->height = height;

	ScreenVertex screenQuad[4] =
	{
		{ { position.x, position.y - height, 0.0f }, { 0.0f, 1.0f } }, // BOTTOM LEFT
		{ { position.x, position.y, 0.0f }, { 0.0f, 0.0f } },   // TOP LEFT
		{ { position.x + width, position.y, 0.0f }, { 1.0f, 0.0f } },   // TOP RIGHT
		{ { position.x + width, position.y - height, 0.0f }, { 1.0f, 1.0f } }    // BOTTOM RIGHT
	};

	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	Graphics::GetContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &screenQuad, sizeof(screenQuad));
	Graphics::GetContext()->Unmap(this->vertexBuffer, 0);
}

void Decal::RePosition(float x, float y)
{
	position.x = x;
	position.y = y;

	ScreenVertex screenQuad[4] =
	{
		{ { position.x, position.y - (float)width, 0.0f }, { 0.0f, 1.0f } }, // BOTTOM LEFT
		{ { position.x, position.y, 0.0f }, { 0.0f, 0.0f } },   // TOP LEFT
		{ { position.x + (float)width, position.y, 0.0f }, { 1.0f, 0.0f } },   // TOP RIGHT
		{ { position.x + (float)width, position.y - (float)height, 0.0f }, { 1.0f, 1.0f } }    // BOTTOM RIGHT
	};

	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	Graphics::GetContext()->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &screenQuad, sizeof(screenQuad));
	Graphics::GetContext()->Unmap(this->vertexBuffer, 0);
}

void Decal::Render()
{
	UINT stride = sizeof(ScreenVertex);
	UINT offset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
	CONTEXT->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	CONTEXT->PSSetShaderResources(0, 1, &this->texture->GetShaderView());

	/*
		Draw!!
	*/
	CONTEXT->DrawIndexed(6, 0, 0);
}

const bool Decal::Colliding(float* x, float* y)
{
	// In this case, x and y is the position of the mouse.
	float x_pos = *(x);
	float y_pos = *(y);

	/*
	std::cout << "X: " << x_pos << " Y: " << y_pos << "\n" << "X2: " << position.x
		<< " Y2: " << position.y << "\n" << "Width: " << position.x + width << " Height: " <<
		position.y + height << "\n";
		*/

	if (x_pos >= position.x && x_pos <= position.x + width &&
		y_pos <= position.y && y_pos >= position.y - height)
		return true;

	return false;
}
