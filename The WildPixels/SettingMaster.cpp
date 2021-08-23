#include "SettingMaster.h"

SettingMaster* SettingMaster::instance = nullptr;
#define INSTANCE SettingMaster::instance

SettingMaster::SettingMaster()
{
	this->updateAvailable = false;
	this->windowed = false;
	this->winHeight = 0;
	this->winWidth = 0;
	this->current_game_state = nullptr;
	this->hInstance = {};
}

SettingMaster::~SettingMaster()
{
}

void SettingMaster::UpdateResolution()
{
	if (INSTANCE->current_game_state == nullptr)
		return;
}

void SettingMaster::Initialize(HINSTANCE hInstance, UINT width, UINT height)
{
	if (INSTANCE)
	{
		delete INSTANCE;
	}

	INSTANCE = new SettingMaster();
	INSTANCE->winHeight = height;
	INSTANCE->winWidth = width;
	INSTANCE->hInstance = hInstance;
}

void SettingMaster::SetGame(Game* game_pointer)
{
	INSTANCE->current_game_state = game_pointer;
}
