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

struct Vector2D
{
	float x;
	float y;

	Vector2D(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2D()
	{
		this->x = 0;
		this->y = 0;
	}
};

struct Vector3D
{
	float x;
	float y;
	float z;

	Vector3D(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3D()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
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

/*
	Pos.w == type
	direction.w == ?
	attenuation.w = ?
*/
struct LightStruct
{
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 direction;
	DirectX::XMFLOAT4 attenuation;
};

struct LightProperties
{
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 specular;
	DirectX::XMFLOAT4 diffuse;

	LightProperties(float amb, float spec, float diff)
	{
		this->ambient = { amb, amb, amb, amb };
		this->specular = { spec, spec, spec, spec };
		this->diffuse = { diff, diff, diff, diff };
	}

	LightProperties(DirectX::XMFLOAT4 amb, DirectX::XMFLOAT4 spec, DirectX::XMFLOAT4 diff)
	{
		this->ambient = amb;
		this->specular = spec;
		this->diffuse = diff;
	}
};