#include "Enemy.h"

Enemy::Enemy()
{
	this->stats = {};
	this->position = {};

	this->body = new MeshObject();
}

Enemy::~Enemy()
{
}

MeshObject* Enemy::GetMesh() const
{
	return this->body;
}
