#include "Light.h"
#include "Graphics.h"

bool Light::setup_light_rules()
{
	D3D11_BUFFER_DESC bDesc;
	bDesc.ByteWidth = sizeof(general_light_rules);
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;

	// 1 - amount of lights, 2 - range of light render, 3 - ?, 4 - ?
	this->light_rules.package = { 0.0f, 300.0f, 0.0f, 0.0f };
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &(this->light_rules);

	HRESULT hr = DEVICE->CreateBuffer(&bDesc, &data, &this->light_rules_buffer);
	return !FAILED(hr);
}

bool Light::setup_light_buffers()
{
	check_buffers();

	if ((UINT)this->light_structs.size() <= 0 && (UINT)this->light_props.size() <= 0)
		return false;

	// Init buffers.
	HRESULT hr;

	D3D11_BUFFER_DESC bDesc;
	bDesc.ByteWidth = sizeof(LightStruct) * (UINT)this->light_structs.size();
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bDesc.StructureByteStride = sizeof(LightStruct);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &(this->light_structs[0]);

	hr = DEVICE->CreateBuffer(&bDesc, &data, 
		&this->light_structs_buffer);

	if (SUCCEEDED(hr))
	{
		// Create resource view.
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.BufferEx.FirstElement = 0;
		srvDesc.BufferEx.Flags = 0;
		srvDesc.BufferEx.NumElements = (UINT)this->light_structs.size();

		hr = DEVICE->CreateShaderResourceView(this->light_structs_buffer, &srvDesc, &this->light_structs_view);

		if (FAILED(hr))
			return false;
	}
	else
		return false;

	// Next buffer.

	bDesc.ByteWidth = sizeof(LightProperties) * (UINT)this->light_props.size();
	bDesc.StructureByteStride = sizeof(LightProperties);

	data.pSysMem = &(this->light_props[0]);

	hr = DEVICE->CreateBuffer(&bDesc, &data,
		&this->light_props_buffer);

	if (SUCCEEDED(hr))
	{
		// Create resource view.
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.BufferEx.FirstElement = 0;
		srvDesc.BufferEx.Flags = 0;
		srvDesc.BufferEx.NumElements = (UINT)this->light_props.size();

		hr = DEVICE->CreateShaderResourceView(this->light_props_buffer, &srvDesc, &this->light_props_view);

		if (FAILED(hr))
			return false;
	}
	else
		return false;

	return true;
}

void Light::check_buffers()
{
	if (this->light_structs_buffer)
		this->light_structs_buffer->Release();
	if (this->light_structs_view)
		this->light_structs_view->Release();

	if (this->light_props_buffer)
		this->light_props_buffer->Release();
	if (this->light_props_view)
		this->light_props_view->Release();
}

void Light::Cleanup()
{

}

void Light::InternalRender()
{

}

void Light::UpdateRules()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	Graphics::GetContext()->Map(this->light_rules_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &this->light_rules, sizeof(general_light_rules));
	Graphics::GetContext()->Unmap(this->light_rules_buffer, 0);
}

Light::Light()
{
	this->light_rules = {};
	this->light_props_buffer = nullptr;
	this->light_rules_buffer = nullptr;
	this->light_structs_buffer = nullptr;
	this->light_structs_view = nullptr;
	this->light_props_view = nullptr;
	this->update = false;
}

Light::~Light()
{
	if (this->light_props_buffer)
		this->light_props_buffer->Release();
	if (this->light_rules_buffer)
		this->light_rules_buffer->Release();
	if (this->light_structs_buffer)
		this->light_structs_buffer->Release();
	if (this->light_structs_view)
		this->light_structs_view->Release();
	if (this->light_props_view)
		this->light_props_view->Release();
}

void Light::Setup()
{
	bool result = setup_light_rules();

	// Try to setup buffers and shader views.
	if (result)
	{
		setup_light_buffers();
	}
}

void Light::SetLightDistance(const float&& dist)
{
	this->light_rules.package.y = dist;
	UpdateRules();
}

void Light::AddLight(LightStruct light_struct, LightProperties light_props)
{
	this->update = true;
}

void Light::AddLight(LightStruct light_struct)
{
	this->update = true;

	this->light_structs.push_back(light_struct);

	LightProperties default_properites(1.0f, 1.0f, 1.0f);
	this->light_props.push_back(default_properites);
}

void Light::Render()
{
	if (this->update)
	{
		setup_light_buffers();
		this->update = false;
	}
	this->InternalRender();
	this->Cleanup();
}
