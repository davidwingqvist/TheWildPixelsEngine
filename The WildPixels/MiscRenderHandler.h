#pragma once
#include "RenderAble.h"
#include "Decal.h"
#include "Button.h"
#include "Canvas.h"
#include <vector>
#include <d3d11.h>

class MiscRenderer
{
private:

	ID3D11PixelShader* pixelShader;
	ID3D11VertexShader* vertexShader;
	ID3D11SamplerState* sampler;

	void Setup();
	void Bind();
	void UnBind();

private:

	// Objects to render onto screen.
	std::vector<Renderable*> renderList;
	MiscRenderer();
	~MiscRenderer();

public:

	static MiscRenderer* instance;
	static void Initialize();
	static void Destroy();
	static void AddRenderObject(Renderable* object);
	static Renderable* GetRenderObject(unsigned int pos);
	static int GetVectorSize();
	static void Render();
};

#define ADD_MISC_REND(renderObject) MiscRenderer::AddRenderObject(renderObject)
#define GET_MISC_RENDER(type, pos) dynamic_cast<type*>(MiscRenderer::GetRenderObject(pos))
#define MISC_RENDER MiscRenderer::Render();