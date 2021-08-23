#pragma once
#include <d3d11.h>
#include "Game.h"

class SettingMaster
{
private:

	static SettingMaster* instance;
	UINT winWidth, winHeight;
	bool windowed;
	bool updateAvailable;
	SettingMaster();
	~SettingMaster();
	Game* current_game_state;
	HINSTANCE hInstance;

public:

	static void Initialize(HINSTANCE hInstance, UINT width, UINT height);
	static void UpdateResolution();
	static void SetGame(Game* game_pointer);
	static void Destroy();
	static void SetResolution(UINT width, UINT height);
};