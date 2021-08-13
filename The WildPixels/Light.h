#pragma once
#include "Structures.h"
#include <d3d11.h>
#include <vector>

class Light
{
private:

	// Here put how many lights there are, other important things.
	struct general_light_rules
	{
		DirectX::XMFLOAT4 package;
	}light_rules;

	std::vector<LightStruct> light_structs;
	std::vector<LightProperties> light_props;

	ID3D11Buffer* light_structs_buffer;
	ID3D11Buffer* light_props_buffer;
	ID3D11ShaderResourceView* light_structs_view;
	ID3D11ShaderResourceView* light_props_view;

	ID3D11Buffer* light_rules_buffer;

	bool setup_light_rules();
	bool setup_light_buffers();

	// check if any of the buffers are created and destory them.
	void check_buffers();


	void InternalRender();
	void UpdateRules();
	
	void thread_setup_light_buffers();

	bool update, internalUpdate;

public:

	Light();
	~Light();

	void Setup();

	// Set the render distance for lights.
	void SetLightDistance(const float&& dist);

	// Add a light to the buffer.
	void AddLight(LightStruct light_struct, LightProperties light_props);

	// This will automate the properties of the light to default values.
	void AddLight(LightStruct light_struct);

	void Render();

	// Release any buffers or resources from the pipeline.
	void Cleanup();
};