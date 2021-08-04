#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
class GameObject
{
protected:

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	ID3D11Buffer* modelMatrixBuffer;
	ID3D11Buffer* vertexBuffer;

private:

	void CreateModelBuffer();

public:

	GameObject();
	virtual ~GameObject();

	void Create(DirectX::XMFLOAT3 position = { 0, 0, 0 }, DirectX::XMFLOAT3 rotation = {0, 0, 0},
		DirectX::XMFLOAT3 scale = {1.0f, 1.0f, 1.0f});

};