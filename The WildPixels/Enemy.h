#pragma once
#include "MeshObject.h"

class Enemy
{
private:

	struct Stats
	{
		int hp;
		float speed;
	}stats;

	struct Position
	{
		float x, y, z;
	}position;

	MeshObject* body;

public:

	Enemy();
	~Enemy();

	MeshObject* GetMesh() const;
};