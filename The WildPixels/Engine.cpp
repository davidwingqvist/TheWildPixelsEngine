#include "Engine.h"
#include <io.h>
#include "ResourceManager.h"

void Engine::RedirectIoToConsole()
{
	AllocConsole();
	HANDLE stdHandle;
	int hConsole;
	FILE* fp;
	stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	hConsole = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
	fp = _fdopen(hConsole, "w");

	freopen_s(&fp, "CONOUT$", "w", stdout);
}

// --------DEFERRED--------
void Engine::RenderDeferred()
{
	double begin;
	double end;

	begin = omp_get_wtime();
	Graphics::ClearScreen();
	end = omp_get_wtime() - begin;
	//std::cout << "Time: " << end << "\n";

	this->renderer.PrepareGeometryPass();

	
	
	if (this->currentCamera != nullptr)
		this->currentCamera->Render();
	

	switch (this->currentState)
	{
	case GameState::NONE:
		break;
	case GameState::INGAME:

		
		this->sceneHandler.RenderScene();
		
		
		
		this->renderer.UnbindGeometryPass();
		
		
		
		this->renderer.RenderLightPass();
		this->renderer.UnbindLightPass();
		
		
		break;
	case GameState::INGAME_OPTIONS:
		break;
	case GameState::MENU:
		break;
	case GameState::OPTIONS:
		break;
	default:
		break;
	}

	
	Graphics::Present();
	
}

// ---------FORWARD----------
void Engine::RenderForward()
{
	Graphics::ClearScreen();
}

Engine::Engine()
{
	this->currentState = GameState::NONE;
	this->currentPlayer = nullptr;
	this->currentCamera = nullptr;
	this->engineRenderType = RenderType::NONE;

	this->isLoading = false;
	this->hasLoadingScreen = false;
}

Engine::~Engine()
{
	Graphics::Destroy();
	MultiThreader::Destroy();
	ResourceManager::Destroy();

	if (this->currentCamera)
		delete this->currentCamera;

	if (this->currentPlayer)
		delete this->currentPlayer;
}

bool Engine::SetUp(HINSTANCE instance, UINT width, UINT height)
{
//#ifdef _DEBUG
	this->RedirectIoToConsole();
//#endif

	// If you don't want multithreading, simply put "amount" to 0.
	MultiThreader::Init(2, ThreadType::POOL_FIFO);
	ResourceManager::Initialize();

	if (!Graphics::Initialize(instance, width, height))
		return false;

	if (!this->renderer.Initialize())
		return false;

	// Default loading screen.
	Engine::SetUpLoadingScreen("");

	return true;
}

bool Engine::SetUpLoadingScreen(std::string&& filePath)
{
	// Default texture.
	if (filePath.size() <= 1)
		filePath = "Textures/buffercat.jpg";

	this->hasLoadingScreen = this->renderer.InitializeLoadingScreen(filePath);

	return this->hasLoadingScreen;
}

bool Engine::SetCamera(Camera* cam)
{
	if (cam != nullptr)
		// No camera existed, point to input.
		if(this->currentCamera == nullptr)
			this->currentCamera = cam;
		else
		{
			// Delete old and replace with new.
			delete this->currentCamera;
			this->currentCamera = cam;
		}
	else
		// Input was nullptr.
		return false;

	return true;
}

Camera* Engine::GetCamera() const
{
	return this->currentCamera;
}

const bool Engine::IsLoading() const
{
	return this->isLoading;
}

void Engine::Render()
{

	switch (this->engineRenderType){
	case RenderType::DEFERRED:
		this->RenderDeferred();
		break;
	case RenderType::FORWARD:
		this->RenderForward();
		break;
	default:
		break;
	}
	
}

void Engine::RenderLoading()
{
	if(this->hasLoadingScreen)
		this->renderer.LoadingScreen();
}

void Engine::SetLoadingStatus(const bool&& status)
{
	this->isLoading = status;
}

SceneManager& Engine::SceneHandle()
{
	return this->sceneHandler;
}

void Engine::RegisterMesh(MeshObject* mesh)
{
	this->sceneHandler.EditScene().Add(mesh);
}

bool Engine::HandleExceptionRendering()
{
	if (IsLoading())
	{

		Engine::RenderLoading();
		Graphics::Present();
		return true;
	}

	return false;
}

void Engine::ClearResources()
{
	ResourceManager::ClearCache();
}

void Engine::SetRenderType(RenderType &&type)
{
	this->engineRenderType = type;
}

void Engine::SetPlayer(Player* player)
{
	this->currentPlayer = player;
}
