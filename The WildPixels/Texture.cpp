#include "Texture.h"
#include "stb_image.h"
#include "Graphics.h"

Texture::Texture(std::string&& filePath)
	:Resource(ResourceType::TEXTURE)
{
	this->texture = nullptr;
	this->textureShader = nullptr;

	int width, height, channels;
	unsigned char* image = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	// If image load failed, return.
	if (width <= 0)
		return;

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
	hr = DEVICE->CreateTexture2D(&tDesc, &data, &this->texture);
	if(SUCCEEDED(hr))
		DEVICE->CreateShaderResourceView(this->texture, 0, &this->textureShader);

	stbi_image_free(image);
}

Texture::~Texture()
{
	if (this->texture)
		this->texture->Release();
	if (this->textureShader)
		this->textureShader->Release();
}

ID3D11ShaderResourceView*& Texture::GetShaderView()
{
	return this->textureShader;
}