#include "EnemyManager.h"

EnemyManager::EnemyManager()
{

}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::AddEnemy(Enemy* enemy)
{
	this->enemies.push(enemy);
}

void EnemyManager::RemoveEnemy(unsigned int index)
{
	this->enemies.set_top(index);
	this->enemies.pop();
}

Enemy* EnemyManager::GetEnemy(unsigned int index)
{
	return this->enemies[index];
}
