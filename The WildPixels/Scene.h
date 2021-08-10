#pragma once
#include <vector>
#include "MeshObject.h"
#include "Light.h"
class Scene
{
private:

	std::vector<MeshObject*> objects;

	Light* sceneLights;

	void RenderInternal();

public:

	Scene();
	virtual ~Scene();

	void Add(MeshObject* obj);
	void Add(std::string meshPath = "BlockyAK47.obj", std::string texture = "", 
		DirectX::XMFLOAT3 position = { 0, 0, 0 }, DirectX::XMFLOAT3 rotation = { 0, 0, 0 },
		DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f });

	MeshObject* GetObj(unsigned int index);
	void Render();
	void RenderLights();
};

