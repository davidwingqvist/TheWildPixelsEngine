#pragma once
#include "Storage.h"
#include "Enemy.h"

class EnemyManager
{
private:

	storage::Link<Enemy*> enemies;

public:

	EnemyManager();
	~EnemyManager();

	void AddEnemy(Enemy* enemy);
	void RemoveEnemy(unsigned int index);

	Enemy* GetEnemy(unsigned int index);
};