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
		for (unsigned int i = 0; i < (unsigned int)MiscRenderer::GetVectorSize(); i++)
		{
			Decal* decal = GET_MISC_RENDER(Decal, i);
			Button* button = GET_MISC_RENDER(Button, i);
			if (decal)
				if (decal->Colliding(MOUSE_X, MOUSE_Y) && decal->IsClickAble())
				{
					//decal->RePosition(randomize(-1.0f, 0.0f), randomize(0.0f, 1.0f));
				}
			if (button)
				if (button->Colliding(MOUSE_X, MOUSE_Y))
				{
					button->Toggle();
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

	Camera* cam = new Camera();
	cam->Preset();
	Engine::SetCamera(cam);
	this->currentState = GameState::INGAME;

//#ifdef _DEBUG
	std::cout << "[THREAD/GAME]Loaded Main Menu..." << "\n";
//#endif

	SetLoadingStatus(false);
}

void Game::UI()
{
	/*
	Decal* decal = new Decal("Textures/Theunit.png", .5f, .0f, .25f, .25f);
	decal->ToggleClickAble(true);
	ADD_MISC_REND(decal);

	Decal* decal2 = new Decal("Textures/buffercat.jpg", -1.0f, 1.0f, .25f, .25f);
	decal2->ToggleClickAble(true);
	ADD_MISC_REND(decal2);
	*/
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
	THREAD_JOB(Game, UI);

	Canvas* canvas = new Canvas(-0.5f, .5f, 1.0f, 1.0f, .76f, 0.255f, 0.3f);
	canvas->ApplyTexture("Textures/bignose.jpg");
	ADD_MISC_REND(canvas);

	Button* button = new Button(-.20f, 0.35f);
	button->ApplyTexture("Textures/black.png");
	button->SetAsToggle();

	ADD_MISC_REND(button);

	Button* button2 = new Button(-.20f, -.0f);
	button2->ApplyTexture("Textures/black.png");
	button2->SetAsToggle();

	ADD_MISC_REND(button2);

	Text2D* text = new Text2D("Text2D class object!");
	text->SetPosition(-150.0f, 0.0f);

	ADD_MISC_REND(text);
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
