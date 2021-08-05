#include "Game.h"
#include <cstring>

/*
	---------INPUTS---------
*/

void Game::HandleMenuInput()
{

}

void Game::HandleInGameInput()
{
	/*
		Movement
	*/
	if (INPUT(keyboard::KEY_W))
	{
		GetCamera()->GetParts().position.x += 3.0f * Graphics::deltaTime * GetCamera()->CameraForward().x;
		GetCamera()->GetParts().position.z += 3.0f * Graphics::deltaTime * GetCamera()->CameraForward().z;
		GetCamera()->UpdateDOOM();
	}
	else if (INPUT(keyboard::KEY_S))
	{
		GetCamera()->GetParts().position.x -= 3.0f * Graphics::deltaTime * GetCamera()->CameraForward().x;
		GetCamera()->GetParts().position.z -= 3.0f * Graphics::deltaTime * GetCamera()->CameraForward().z;
		GetCamera()->UpdateDOOM();
	}

	if (INPUT(keyboard::KEY_A))
	{
		GetCamera()->CamRotation().y -= 3.0f * Graphics::deltaTime;
		GetCamera()->UpdateDOOM();
	}
	else if (INPUT(keyboard::KEY_D))
	{
		GetCamera()->CamRotation().y += 3.0f * Graphics::deltaTime;
		GetCamera()->UpdateDOOM();
	}

	if (INPUT(VK_SPACE))
	{
		GetCamera()->GetParts().position.y += 3.0f * Graphics::deltaTime;
		GetCamera()->UpdateDOOM();
	}
	else if (INPUT(VK_LSHIFT))
	{
		GetCamera()->GetParts().position.y -= 3.0f * Graphics::deltaTime;
		GetCamera()->UpdateDOOM();
	}

	if (PRESSED(VK_LBUTTON))
	{
		GetCamera()->GetParts().position.y += 3.0f * Graphics::deltaTime;
		GetCamera()->UpdateDOOM();
	}
}

/*
	----------UPDATES---------
*/

void Game::UpdateMenu()
{

}

void Game::UpdateInGame()
{

}

void Game::LoadObjects()
{
	// Designed to take time.
	for (int i = 1; i < 500; i++)
	{
		EDITSCENE.Add("clue_toy.obj", "", { (float)(rand() % i), (float)(rand() % i), (float)(rand() % i) }, { (float)(rand() % i),  (float)(rand() % i), (float)(rand() % i) });
	}
}
/*
	--------SCENE LOADS---------
*/
void Game::LoadMainMenu()
{
	SetLoadingStatus(true);
	this->player = new Player();
	Engine::SetPlayer(this->player);
	
	SceneHandle().RemoveAllScenes();
	SceneHandle().AddScene();

	// Designed to take time.
	for (int i = 1; i < 500; i++)
	{
		EDITSCENE.Add("clue_toy.obj", "", { (float)(rand() % i), (float)(rand() % i), (float)(rand() % i) }, { (float)(rand() % i),  (float)(rand() % i), (float)(rand() % i) });
	}

	THREAD_JOB(Game, LoadObjects);

	Camera* cam = new Camera();
	cam->Preset();
	Engine::SetCamera(cam);
	this->currentState = GameState::INGAME;

//#ifdef _DEBUG
	std::cout << "[THREAD/GAME]Loaded Main Menu..." << "\n";
//#endif

	SetLoadingStatus(false);
}

Game::Game()
{
	this->player = nullptr;
}

Game::~Game()
{
	delete this->player;
	Engine::SetPlayer(nullptr);
	SceneHandle().RemoveAllScenes();
}

bool Game::StartUp(HINSTANCE instance, UINT width, UINT height)
{
	if (!Engine::SetUp(instance, width, height))
    {
        return false;
    }
	Engine::SetRenderType(RenderType::DEFERRED);
	
	// Load in the main menu at the beginning of game.

	//Engine::ClearResources();
	THREAD_JOB(Game, LoadMainMenu);
	//LoadMainMenu();

    return true;
}

bool Game::Update(float& dt)
{
	
	Graphics::deltaTime = dt;
	
	if (HandleExceptionRendering())
		return true;
	
	this->HandleInput();
	switch (this->currentState)
	{
	case GameState::NONE:
		break;
	case GameState::INGAME:
		this->UpdateInGame();
		break;
	case GameState::INGAME_OPTIONS:
		break;
	case GameState::MENU:
		this->UpdateMenu();
		break;
	case GameState::OPTIONS:
		break;
	default:
		break;
	}
	
	

	Engine::Render();
	
	
    return true;
}

bool Game::HandleInput()
{
	/*
		Different Input behavior for different circumstances.
	*/
	switch (this->currentState)
	{
	case GameState::NONE:
		break;
	case GameState::INGAME:
		this->HandleInGameInput();
		break;
	case GameState::INGAME_OPTIONS:
		break;
	case GameState::MENU:
		this->HandleMenuInput();
		break;
	case GameState::OPTIONS:
		break;
	default:
		break;
	}
	return false;
}
