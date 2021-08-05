#pragma once
#include "Engine.h"
#include "EnemyManager.h"

class Game : public Engine
{
private:

	// Inputs
	void HandleMenuInput();
	void HandleInGameInput();

private:

	// Updates
	void UpdateMenu();
	void UpdateInGame();

private:

	Player* player;
	EnemyManager enemyManager;

	void LoadMainMenu();

public:
	
	Game();
	~Game();

	bool StartUp(HINSTANCE instance, UINT width, UINT height);
	bool Update(float& dt);
	bool HandleInput();
	void LoadObjects();
};

