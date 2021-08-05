#include "MeshObject.h"
#include "ReadFromObj.h"
#include "Graphics.h"

MeshObject::MeshObject()
	:GameObject()
{
	this->texture = nullptr;
	this->mesh = nullptr;
}

MeshObject::~MeshObject()
{
}

void MeshObject::Create(std::string meshPath, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale)
{
	GameObject::Create(position, rotation, scale);

	this->mesh = RESOURCE(Mesh, std::string("Models/" + meshPath));
	if (!this->mesh)
	{
		this->mesh = new Mesh(std::string("Models/" + meshPath));
		ADD_RESOURCE(mesh, "Models/" + meshPath);
	}
}

void MeshObject::CreateTexture(std::string &&filePath)
{
	if (filePath == "Textures/")
	{
		// Default skin.
		filePath = "Textures/bignose.jpg";
	}

	this->texture = RESOURCE(Texture, std::string(filePath));
	if (!this->texture)
	{
		this->texture = new Texture(std::string(filePath));
		ADD_RESOURCE(this->texture, filePath);
	}
}

void MeshObject::Render()
{
	if (this->mesh)
	{
		UINT offset = 0;
		UINT stride = sizeof(Vertex);
		CONTEXT->IASetVertexBuffers(0, 1, &this->mesh->GetMesh(), &stride, &offset);
		CONTEXT->VSSetConstantBuffers(0, 1, &this->modelMatrixBuffer);

		if (this->texture)
			CONTEXT->PSSetShaderResources(0, 1, &this->texture->GetShaderView());

		CONTEXT->Draw(this->mesh->GetVertCount(), 0);
	}
}
