#pragma once
#include <d3d11.h>
#include <string>
#include "Structures.h"
#include "RenderAble.h"
#include "ResourceManager.h"
#include "Camera.h"

class Decal : public Renderable
{
private:

	Texture* texture;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	Vector2D position;
	float width;
	float height;

	void CreateTexture(const std::string file_path);
	bool CreateVertexBuffer(float* x, float* y);
	bool CreateVertexBuffer(float* x, float* y, float width, float height);

	void Unbind();
public:

	// Create a decal to be rendered at the center of the screen.
	Decal(const std::string&& decal_path);

	// Create a decal to be rendered at the specified location.
	Decal(const std::string&& decal_path, float x, float y);
	Decal(const std::string&& decal_path, Vector2D& pos);
	Decal(const std::string&& decal_path, float x, float y, float width, float height);

	Decal();
	virtual ~Decal() override;

	void Resize(float width, float height);
	void RePosition(float x, float y);

	virtual void Render() override;

	/*
		Keep in mind that coordinates needs to be in NDC space.
	*/
	virtual const bool Colliding(float* x, float* y) override;
};