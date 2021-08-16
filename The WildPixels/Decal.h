#pragma once
#include <d3d11.h>
#include <string>
#include "Structures.h"

class Decal
{
private:

	ID3D11Texture2D* decalTexture;
	ID3D11ShaderResourceView* decalShaderView;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	bool doRender;
	Vector2D position;

	bool CreateTexture(const std::string file_path);
	bool CreateVertexBuffer(float* x, float* y);
	bool CreateVertexBuffer(float* x, float* y, float width, float height);
public:

	// Create a decal to be rendered at the center of the screen.
	Decal(const std::string&& decal_path);

	// Create a decal to be rendered at the specified location.
	Decal(const std::string&& decal_path, float x, float y);
	Decal(const std::string&& decal_path, Vector2D& pos);

	Decal();
	~Decal();

	void Resize(float width, float height);

	void ToggleRender(const bool&& toggle);
	const bool IsRender() const;
	void Render();
};