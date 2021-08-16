#pragma once
#include "Window.h"
#include <omp.h>
#include "Renderer.h"
#include "Graphics.h"
#include "GameState.h"
#include "Player.h"
#include "SceneManager.h"
#include "multi_thread_manager.h"
#include "Storage.h"
#include "Camera.h"
#include "Decal.h"
#include <iostream>
#include <fcntl.h>
#include "InputHandler.h"
#pragma comment(lib, "Winmm.lib")

//	DEFINES

#define EDITSCENE SceneHandle().EditScene()
#define MESH(mesh) Engine::RegisterMesh(mesh)
#define INPUT(key) Engine::inputHandler.IsHeld(key)
#define PRESSED(key) Engine::inputHandler.IsPressedDown(key)

enum class RenderType {

	// Renders nothing.
	NONE,

	// Deferred rendering.
	DEFERRED,

	// Forward rendering.
	FORWARD,

	// Number of rendering types.
	NROF
};

class Engine
{
protected:
	GameState currentState;
private:

	Renderer renderer;
	SceneManager sceneHandler;
	Player* currentPlayer;

	Camera* currentCamera;
	RenderType engineRenderType;

	// Keeps track of if the engine should render a loading screen or not.
	bool isLoading;
	bool hasLoadingScreen;

	void RedirectIoToConsole();

	void RenderDeferred();
	void RenderForward();

protected:

	InputHandler inputHandler;

public:


	Engine();
	~Engine();

	bool SetUp(HINSTANCE instance, UINT width, UINT height);
	bool SetUpLoadingScreen(std::string&& filePath);

	/* 
		Transfer the pointer of Camera into the Engine. Engine will later take care of
		deleting the allocated space for camera.
	*/
	bool SetCamera(Camera* cam);
	Camera* GetCamera() const;

	/*
		Set the preferred render type for your game.
		Deferred is generally better for more lights.
		Forward is usually better for more managed memory usage.
	*/
	void SetRenderType(RenderType &&type);
	void SetPlayer(Player* player);

	// Kindly ask the engine if it is loading.
	const bool IsLoading() const;

	void Render();
	void RenderLoading();

	// Set the engine to render loading screen, remember to set to false when all loading is done.
	void SetLoadingStatus(const bool&& status);

	SceneManager& SceneHandle();
	void RegisterMesh(MeshObject* mesh);

	// This will enable rendering such as loading screen, etc.
	bool HandleExceptionRendering();

	/*
		Storage handling
	*/

	// Remove every Resources currently emplaced in ResourceManager.
	void ClearResources();
};


