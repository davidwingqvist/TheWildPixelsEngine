#pragma once
#include <d3d11.h>
#include "Structures.h"
#include <string>
#include "Texture.h"

constexpr int BUFFERS = 2;

class Renderer
{
private:

	/*
	
		FORWARD RENDERING
	
	*/
private:

	/*
	
		DEFERRED RENDERING

	*/

	/*
		Geometry Pass
	*/
	ID3D11RenderTargetView* renderTargetViews[BUFFERS];
	ID3D11ShaderResourceView* shaderResourceViews[BUFFERS];
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* textures[BUFFERS];
	ID3D11InputLayout* inputLayout;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	D3D11_VIEWPORT viewPort;
	ID3D11RasterizerState* rastState;

	// Can be used for both Geo and Light
	ID3D11SamplerState* sampler;

	void SetupGeometryPass();
	
private:

	/*
		Light Pass
	*/

	ID3D11Texture2D* lightTexture;
	ID3D11Buffer* lightVertexBuffer;
	ID3D11VertexShader* lightVertexShader;
	ID3D11PixelShader* lightPixelShader;
	ID3D11InputLayout* lightInputLayout;
	ID3D11Buffer* indexBuffer;

	void SetupLightPass();

private:

	/*
		LOADING SCREEN RENDER
	*/
	ID3D11VertexShader* loadingVertexShader;
	ID3D11PixelShader* loadingPixelShader;
	Texture* loadTexture;
	std::string filePath;

	void CheckLoaded();

	// Load a texture through the resource manager
	void LoadTexture();

public:

	Renderer();
	~Renderer();

	bool Initialize();
	bool InitializeLoadingScreen(std::string filePath);
	void PrepareGeometryPass();
	void UnbindGeometryPass();
	void RenderLightPass();
	void UnbindLightPass();

	// Renders a loading screen.
	void LoadingScreen();

};