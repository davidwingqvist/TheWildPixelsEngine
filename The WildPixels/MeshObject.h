#pragma once
#include "GameObject.h"
#include "Structures.h"
#include <string>
#include <memory>
#include "stb_image.h"
#include "ResourceManager.h"

class MeshObject :
    public GameObject
{
private:

    Texture* texture;
    Mesh* mesh;
public:

    MeshObject();
    ~MeshObject();

    void Create(std::string meshPath = "cube.obj", DirectX::XMFLOAT3 position = { 0, 0, 0 }, DirectX::XMFLOAT3 rotation = { 0, 0, 0 },
        DirectX::XMFLOAT3 scale = { 0, 0, 0 });

    void CreateTexture(std::string &&filePath = "");

    void Render();
};

