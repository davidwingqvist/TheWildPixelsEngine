#include "MiscRenderHandler.h"
#include <fstream>
#include "Graphics.h"
#include "multi_thread_manager.h"

MiscRenderer* MiscRenderer::instance = nullptr;
#define RENDERER MiscRenderer::instance

void MiscRenderer::Setup()
{
	std::string shaderData;
	std::ifstream reader;

	reader.open("MiscRenderVertexShader.cso", std::ios::binary | std::ios::ate);
	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	Graphics::GetDevice()->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &this->vertexShader);

	std::string shaderData2;
	std::ifstream reader2;

	reader2.open("MiscRenderPixelShader.cso", std::ios::binary | std::ios::ate);
	reader2.seekg(0, std::ios::end);
	shaderData2.reserve(static_cast<unsigned int>(reader2.tellg()));
	reader2.seekg(0, std::ios::beg);

	shaderData2.assign((std::istreambuf_iterator<char>(reader2)),
		std::istreambuf_iterator<char>());

	Graphics::GetDevice()->CreatePixelShader(shaderData2.c_str(), shaderData2.length(), nullptr, &this->pixelShader);
	
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

	Graphics::GetDevice()->CreateSamplerState(&samplerDesc, &this->sampler);
}

void MiscRenderer::Bind()
{
	Graphics::SetBackbufferAsTarget();
	CONTEXT->PSSetShader(this->pixelShader, NULL, NULL);
	CONTEXT->VSSetShader(this->vertexShader, NULL, NULL);
	CONTEXT->PSSetSamplers(0, 1, &this->sampler);
}

void MiscRenderer::UnBind()
{
	ID3D11SamplerState* nullSampler = nullptr;
	CONTEXT->PSSetShader(NULL, NULL, NULL);
	CONTEXT->PSSetSamplers(0, 1, &nullSampler);
}

MiscRenderer::MiscRenderer()
{
	this->pixelShader = nullptr;
	this->vertexShader = nullptr;
	this->sampler = nullptr;
}

MiscRenderer::~MiscRenderer()
{

	while ((int)this->renderList.size() > 0)
	{
		delete this->renderList[(int)this->renderList.size() - 1];
		this->renderList.pop_back();
	}

	if (this->pixelShader)
		this->pixelShader->Release();
	if (this->vertexShader)
		this->vertexShader->Release();
	if (this->sampler)
		this->sampler->Release();
}

void MiscRenderer::Initialize()
{
	if (!RENDERER)
	{
		RENDERER = new MiscRenderer();
		THREAD_SINGLETON_JOB(MiscRenderer, Setup);
	}
}

void MiscRenderer::Destroy()
{
	if (RENDERER)
	{
		delete RENDERER;
	}
}

void MiscRenderer::AddRenderObject(Renderable* object)
{
	RENDERER->renderList.push_back(object);
}

Renderable* MiscRenderer::GetRenderObject(unsigned int pos)
{
	if (pos > (unsigned int)RENDERER->instance->renderList.size() || pos < 0)
		return nullptr;

	return RENDERER->instance->renderList[pos];
}

void MiscRenderer::Render()
{
	// To avoid possible crashes, break when size is less than 0.
	if ((int)RENDERER->renderList.size() <= 0)
		return;
	RENDERER->Bind();
	for (int i = 0; i < (int)RENDERER->renderList.size(); i++)
	{
		// Check if its set to be rendered and render it.
		if(RENDERER->renderList[i]->IsRender())
			RENDERER->renderList[i]->Render();
	}
	RENDERER->UnBind();
}
