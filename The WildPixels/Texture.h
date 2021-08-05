#pragma once
#include "Resource.h"
#include <string>

class Texture : public Resource
{
private:

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* textureShader;
	bool hasLoaded;

public:

	Texture(std::string&& filePath);
	~Texture();

	ID3D11ShaderResourceView*& GetShaderView();
	const bool HasLoaded() const;
};