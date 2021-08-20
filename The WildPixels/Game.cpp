#include "Game.h"
#include <cstring>
#include "gamelogic_math.h"

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
		for (unsigned int i = 0; i < MiscRenderer::GetVectorSize(); i++)
		{
			Decal* decal = GET_MISC_RENDER(Decal, i);
			if (decal)
				if (decal->Colliding(MOUSE_X, MOUSE_Y) && decal->IsClickAble())
				{
					decal->RePosition(randomize(-1.0f, 0.0f), randomize(0.0f, 1.0f));
				}
		}
	}

	UPDATE_INPUT;
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

	EDITSCENE.Add("geo_terrain10x10.obj");
	// Designed to take time.
	for (int i = 1000; i < 5000; i++)
	{
		EDITSCENE.Add("BlockyAK47.obj", "Theunit.png", { (float)(rand() % i), (float)(rand() % i), (float)(rand() % i) }, { (float)(rand() % i),  (float)(rand() % i), (float)(rand() % i) });
	}

	//LightStruct L = {};
	//EDITSCENE.AddLight(L);

	Decal* decal = new Decal("Textures/Theunit.png", .5f, .0f, .1, .1);
	decal->ToggleClickAble(true);
	ADD_MISC_REND(decal);

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
	SceneHandle().RemoveAllScenes();
}

bool Game::StartUp(HINSTANCE instance, UINT width, UINT height)
{
	if (!Engine::SetUp(instance, width, height))
    {
        return false;
    }
	Engine::SetRenderType(RenderType::DEFERRED);

	//Engine::ClearResources();
	THREAD_JOB(Game, LoadMainMenu);

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
