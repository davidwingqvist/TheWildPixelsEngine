#include "Renderer.h"
#include "Graphics.h"
#include "multi_thread_manager.h"
#include <fstream>
#include "stb_image.h"
#include "ResourceManager.h"
#include <omp.h>

void Renderer::SetupGeometryPass()
{
	/*
		Setup Textures, Shader resource views, SamplerState and Render target views
	*/
	HRESULT hr;
	D3D11_TEXTURE2D_DESC desc;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.CPUAccessFlags = 0;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.Height = Graphics::GetHeight();
	desc.Width = Graphics::GetWidth();
	desc.MiscFlags = 0;

	for (int i = 0; i < BUFFERS; i++)
	{
		hr = DEVICE->CreateTexture2D(&desc, NULL, &this->textures[i]);
		if (SUCCEEDED(hr))
		{
			DEVICE->CreateShaderResourceView(this->textures[i], NULL, &this->shaderResourceViews[i]);
			DEVICE->CreateRenderTargetView(this->textures[i], NULL, &this->renderTargetViews[i]);
		}
		else
			return;
	}

	std::string shaderData;
	std::ifstream reader;

	reader.open("GeometryPassVertex.cso", std::ios::binary | std::ios::ate);
	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	hr = Graphics::GetDevice()->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &this->vertexShader);

	std::string shaderData2;
	std::ifstream reader2;

	reader2.open("GeometryPassPixelShader.cso", std::ios::binary | std::ios::ate);
	reader2.seekg(0, std::ios::end);
	shaderData2.reserve(static_cast<unsigned int>(reader2.tellg()));
	reader2.seekg(0, std::ios::beg);

	shaderData2.assign((std::istreambuf_iterator<char>(reader2)),
		std::istreambuf_iterator<char>());

	hr = Graphics::GetDevice()->CreatePixelShader(shaderData2.c_str(), shaderData2.length(), nullptr, &this->pixelShader);

	D3D11_INPUT_ELEMENT_DESC geometryLayout[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	hr = Graphics::GetDevice()->CreateInputLayout(geometryLayout, 3, shaderData.c_str(), shaderData.length(), &this->inputLayout);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 0;

	hr = Graphics::GetDevice()->CreateSamplerState(&samplerDesc, &this->sampler);
	if (FAILED(hr))
	{
		return;
	}

	this->viewPort.Width = (FLOAT)Graphics::GetWidth();
	this->viewPort.Height = (FLOAT)Graphics::GetHeight();
	this->viewPort.MinDepth = 0.0f;
	this->viewPort.MaxDepth = 1.0f;
	this->viewPort.TopLeftX = 0.0f;
	this->viewPort.TopLeftY = 0.0f;

	D3D11_RASTERIZER_DESC rDesc;
	rDesc.AntialiasedLineEnable = false;
	rDesc.FrontCounterClockwise = false;
	rDesc.CullMode = D3D11_CULL_BACK;
	rDesc.DepthBias = 0;
	rDesc.DepthBiasClamp = 0.0f;
	rDesc.DepthClipEnable = true;
	rDesc.FillMode = D3D11_FILL_SOLID;
	rDesc.MultisampleEnable = false;
	rDesc.ScissorEnable = false;
	rDesc.SlopeScaledDepthBias = 0.0f;

	DEVICE->CreateRasterizerState(&rDesc, &this->rastState);
	//CONTEXT->RSSetState(this->rastState);

//#ifdef _DEBUG
	std::cout << "[THREAD/RENDERER]Geometry Pass has been setup.\n";
//#endif
}

void Renderer::SetupLightPass()
{
	ScreenVertex screenQuad[4] =
	{
		{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } }, // BOTTOM LEFT
		{ { -1.0, 1.0f, 0.0f }, { 0.0f, 0.0f } },   // TOP LEFT
		{ { 1.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },   // TOP RIGHT
		{ { 1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } }    // BOTTOM RIGHT
	};

	UINT indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	D3D11_BUFFER_DESC bDesc;
	bDesc.ByteWidth = sizeof(ScreenVertex) * 4;
	bDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.CPUAccessFlags = 0;
	bDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &(screenQuad[0]);
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	DEVICE->CreateBuffer(&bDesc, &data, &this->lightVertexBuffer);

	bDesc.ByteWidth = sizeof(UINT) * 6;
	bDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bDesc.CPUAccessFlags = 0;
	bDesc.MiscFlags = 0;

	data.pSysMem = &(indices[0]);
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	DEVICE->CreateBuffer(&bDesc, &data, &this->indexBuffer);

	std::string shaderData;
	std::ifstream reader;

	reader.open("LightPassVertex.cso", std::ios::binary | std::ios::ate);
	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	Graphics::GetDevice()->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &this->lightVertexShader);

	std::string shaderData2;
	std::ifstream reader2;

	reader2.open("LightPassPixelShader.cso", std::ios::binary | std::ios::ate);
	reader2.seekg(0, std::ios::end);
	shaderData2.reserve(static_cast<unsigned int>(reader2.tellg()));
	reader2.seekg(0, std::ios::beg);

	shaderData2.assign((std::istreambuf_iterator<char>(reader2)),
		std::istreambuf_iterator<char>());

	Graphics::GetDevice()->CreatePixelShader(shaderData2.c_str(), shaderData2.length(), nullptr, &this->lightPixelShader);

	D3D11_INPUT_ELEMENT_DESC geometryLayout[2] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	Graphics::GetDevice()->CreateInputLayout(geometryLayout, 2, shaderData.c_str(), shaderData.length(), &this->lightInputLayout);

//#ifdef _DEBUG
	std::cout << "[THREAD/RENDERER]Light Pass has been setup.\n";
//#endif
	return;
}

void Renderer::CheckLoaded()
{
	if (loadingVertexShader)
		loadingVertexShader->Release();
	if (loadingPixelShader)
		loadingPixelShader->Release();
}

void Renderer::LoadTexture()
{

	this->loadTexture = RESOURCE(Texture, std::string(filePath));
	if (!this->loadTexture)
	{
		this->loadTexture = new Texture(std::string(filePath));
		ADD_RESOURCE(this->loadTexture, filePath);
	}

//#ifdef _DEBUG
	std::cout << "[THREAD/RENDERER]Loading screen: " << filePath << " loaded.\n";
//#endif
}

Renderer::Renderer()
{
	for (int i = 0; i < BUFFERS; i++)
	{
		this->renderTargetViews[i] = nullptr;
		this->textures[i] = nullptr;
		this->shaderResourceViews[i] = nullptr;
	}
	this->depthStencilView = nullptr;
	this->inputLayout = nullptr;
	this->vertexShader = nullptr;
	this->pixelShader = nullptr;
	this->sampler = nullptr;
	this->rastState = nullptr;

	this->lightTexture = nullptr;
	this->lightVertexBuffer = nullptr;
	this->lightVertexShader = nullptr;
	this->lightPixelShader = nullptr;
	this->lightInputLayout = nullptr;
	this->indexBuffer = nullptr;

	this->loadingVertexShader = nullptr;
	this->loadingPixelShader = nullptr;
	this->loadTexture = nullptr;

	this->viewPort = {};
}

Renderer::~Renderer()
{
	for (int i = 0; i < BUFFERS; i++)
	{
		if (this->renderTargetViews[i])
			this->renderTargetViews[i]->Release();
		
		if (this->shaderResourceViews[i])
			this->shaderResourceViews[i]->Release();

		if (this->textures[i])
			this->textures[i]->Release();
	}

	if (this->inputLayout)
		this->inputLayout->Release();
	if (this->vertexShader)
		this->vertexShader->Release();
	if (this->pixelShader)
		this->pixelShader->Release();

	if (this->lightTexture)
		this->lightTexture->Release();

	if (this->lightVertexBuffer)
		this->lightVertexBuffer->Release();

	if (this->lightVertexShader)
		this->lightVertexShader->Release();

	if (this->lightPixelShader)
		this->lightPixelShader->Release();

	if (this->rastState)
		this->rastState->Release();

	if (this->sampler)
		this->sampler->Release();
	if (this->indexBuffer)
		this->indexBuffer->Release();
	if (this->lightInputLayout)
		this->lightInputLayout->Release();
	if (this->loadingVertexShader)
		this->loadingVertexShader->Release();
	if (this->loadingPixelShader)
		this->loadingPixelShader->Release();
}

bool Renderer::Initialize()
{
	THREAD_JOB(Renderer, SetupGeometryPass);
	//SetupGeometryPass();
	THREAD_JOB(Renderer, SetupLightPass);
	//SetupLightPass();

	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}

bool Renderer::InitializeLoadingScreen(std::string filePath)
{
	this->CheckLoaded();

	std::string shaderData;
	std::ifstream reader;
	reader.open("LoadingVertexShader.cso", std::ios::binary | std::ios::ate);
	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	Graphics::GetDevice()->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &this->loadingVertexShader);

	std::string shaderData2;
	std::ifstream reader2;

	reader2.open("LoadingPixelShader.cso", std::ios::binary | std::ios::ate);
	reader2.seekg(0, std::ios::end);
	shaderData2.reserve(static_cast<unsigned int>(reader2.tellg()));
	reader2.seekg(0, std::ios::beg);

	shaderData2.assign((std::istreambuf_iterator<char>(reader2)),
		std::istreambuf_iterator<char>());

	Graphics::GetDevice()->CreatePixelShader(shaderData2.c_str(), shaderData2.length(), nullptr, &this->loadingPixelShader);

	this->filePath = filePath;

	THREAD_JOB(Renderer, LoadTexture);

	return true;
}

void Renderer::PrepareGeometryPass()
{
	
	FLOAT ft[4] = { 0.0f };
	for (int i = 0; i < BUFFERS; i++)
	{
		if(this->renderTargetViews[i])
			CONTEXT->ClearRenderTargetView(this->renderTargetViews[i], ft);
	}

	CONTEXT->VSSetShader(this->vertexShader, nullptr, 0);
	CONTEXT->PSSetShader(this->pixelShader, nullptr, 0);
	CONTEXT->IASetInputLayout(this->inputLayout);
	CONTEXT->OMSetRenderTargets(BUFFERS, this->renderTargetViews, this->depthStencilView);
	CONTEXT->PSSetSamplers(0, 1, &this->sampler);
	CONTEXT->RSSetViewports(1, &this->viewPort);

	/*
		Later on Draw
	*/
}

void Renderer::UnbindGeometryPass()
{
	ID3D11InputLayout* nullLayout = nullptr;
	ID3D11RenderTargetView* nullTargets[BUFFERS] = { nullptr };
	ID3D11DepthStencilView* nullDepth = nullptr;

	CONTEXT->IASetInputLayout(nullLayout);
	CONTEXT->OMSetRenderTargets(BUFFERS, nullTargets, nullDepth);
}

void Renderer::RenderLightPass()
{
	UINT stride = sizeof(ScreenVertex);
	UINT offset = 0;

	CONTEXT->PSSetSamplers(0, 1, &this->sampler);
	CONTEXT->PSSetShaderResources(0, BUFFERS, this->shaderResourceViews);
	CONTEXT->IASetInputLayout(this->lightInputLayout);
	CONTEXT->PSSetShader(this->lightPixelShader, nullptr, 0);
	CONTEXT->VSSetShader(this->lightVertexShader, nullptr, 0);
	CONTEXT->IASetVertexBuffers(0, 1, &this->lightVertexBuffer, &stride, &offset);
	CONTEXT->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Graphics::SetBackbufferAsTarget();

	/*
		Draw screen quad
	*/
	CONTEXT->DrawIndexed(6, 0, 0);
}

void Renderer::UnbindLightPass()
{
	ID3D11InputLayout* nullLayout = nullptr;

	ID3D11RenderTargetView* nullTargets[BUFFERS] = { nullptr };
	ID3D11DepthStencilView* nullDepth = nullptr;

	ID3D11ShaderResourceView* nullShaders[BUFFERS] = { nullptr };
	ID3D11Buffer* indexNull = nullptr;

	//CONTEXT->IASetInputLayout(nullLayout);

	CONTEXT->IASetIndexBuffer(indexNull, DXGI_FORMAT_R32_UINT, 0);
	CONTEXT->OMSetRenderTargets(BUFFERS, nullTargets, nullDepth);
	CONTEXT->PSSetShaderResources(0, BUFFERS, nullShaders);
}

void Renderer::LoadingScreen()
{
	UINT stride = sizeof(ScreenVertex);
	UINT offset = 0;

	CONTEXT->IASetVertexBuffers(0, 1, &this->lightVertexBuffer, &stride, &offset);
	CONTEXT->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	CONTEXT->VSSetShader(this->loadingVertexShader, NULL, 0);
	CONTEXT->PSSetShader(this->loadingPixelShader, NULL, 0);
	CONTEXT->PSSetSamplers(0, 1, &this->sampler);
	CONTEXT->IASetInputLayout(this->lightInputLayout);
	CONTEXT->RSSetViewports(1, &this->viewPort);

	if(this->loadTexture)
		CONTEXT->PSSetShaderResources(0, 1, &this->loadTexture->GetShaderView());

	Graphics::SetBackbufferAsTarget();

	/*
		Draw screen quad
	*/
	CONTEXT->DrawIndexed(6, 0, 0);
}