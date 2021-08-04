#pragma once
#include <DirectXMath.h>
#include <array>

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;

	Vertex(std::array<float, 3> pos, std::array<float, 2> uv, std::array<float, 3> normal)
	{
		this->pos.x = pos[0];
		this->pos.y = pos[1];
		this->pos.z = pos[2];

		this->uv.x = uv[0];
		this->uv.y = uv[1];

		this->normal.x = normal[0];
		this->normal.y = normal[1];
		this->normal.z = normal[2];
	}

	Vertex()
	{
		this->pos = {};
		this->uv = {};
		this->normal = {};
	}
};

struct ScreenVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

struct Matrix
{
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 proj;
};

struct MatrixCompact
{
	DirectX::XMFLOAT4X4 transform;
};