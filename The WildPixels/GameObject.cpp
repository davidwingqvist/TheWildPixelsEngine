#include "GameObject.h"
#include "Graphics.h"
#include "Structures.h"
#include "multi_thread_manager.h"

void GameObject::CreateModelBuffer()
{
	DirectX::XMMATRIX modelMatrix = DirectX::XMMatrixTranspose(
	DirectX::XMMatrixScaling(this->scale.x, this->scale.y, this->scale.z) *	
	DirectX::XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z) *
	DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z)
	);

	D3D11_BUFFER_DESC bDesc;
	bDesc.ByteWidth = sizeof(modelMatrix);
	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &modelMatrix;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	DEVICE->CreateBuffer(&bDesc, &data, &this->modelMatrixBuffer);
}

GameObject::GameObject()
{
	this->modelMatrixBuffer = nullptr;
	this->vertexBuffer = nullptr;
	this->position = {};
	this->rotation = {};
	this->scale = {};
}

GameObject::~GameObject()
{
	if (this->modelMatrixBuffer)
		this->modelMatrixBuffer->Release();
}

void GameObject::Create(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	this->CreateModelBuffer();
}
