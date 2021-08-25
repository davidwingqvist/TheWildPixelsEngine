#include "Game.h"
#include <stdlib.h>
#include <omp.h>
#include <io.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "SettingMaster.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	Game* game = new Game();
	UINT width = 1080;
	UINT height = 720;

	// Disables the notification of program being "unresponsive" but its only loading shit.
	DisableProcessWindowsGhosting();

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//texture2->GetResource(__uuidof(ID3D11Texture2D), (void**)&texture2->texture);
	
	if (!game->StartUp(hInstance, width, height))
	{
		//return 0;
	}
	//SettingMaster::Initialize(hInstance, 1920, 1080);

	MSG msg = { };
	double currentFrame = 0.f, lastFrame = omp_get_wtime();
	float deltaTime = 0.f, deltaSum = 0.f;

	// Desired FPS
	const float targetDelta = 1 / 100000000000000000000000.f;
	while (!(GetKeyState(VK_ESCAPE) & 0x8000) && msg.message != WM_QUIT)
	{
		// If this returns true then the game should quit.
		if (InputHandler::HandleMessages())
			break;

		currentFrame = omp_get_wtime();
		deltaTime = static_cast<float>(currentFrame - lastFrame);
		if (deltaSum >= targetDelta)
		{
			game->Update(deltaSum);
			deltaSum = 0.f;
		}
		deltaSum += deltaTime;
		lastFrame = currentFrame;
	}

	delete game;
	return 0;
}