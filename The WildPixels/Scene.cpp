#include "Scene.h"
#include "multi_thread_manager.h"
#include <iostream>

void Scene::RenderInternal()
{
	if (this->objects.size() > 0)
	{
		// Render each object in scene.
		for (int i = 0; i < (int)this->objects.size(); i++)
		{
			if(this->objects[i] != nullptr)
				this->objects[i]->Render();
		}
	}
}

Scene::Scene()
{

}

Scene::~Scene()
{
	while (this->objects.size() > 0)
	{
		delete this->objects[(int)this->objects.size() - 1];
		this->objects.pop_back();
	}
}

void Scene::Add(MeshObject* obj)
{
	this->objects.push_back(obj);
}

void Scene::Add(std::string meshPath, std::string texture, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale)
{
	MeshObject* newObject = new MeshObject();

	newObject->Create(meshPath, position, rotation, scale);

	newObject->CreateTexture("Textures/" + texture);
	this->objects.push_back(newObject);
}

MeshObject* Scene::GetObj(unsigned int index)
{
	return this->objects[index];
}

void Scene::Render()
{
	this->RenderInternal();
}
