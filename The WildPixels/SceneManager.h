#pragma once
#include "Scene.h"
class SceneManager
{
private:

	std::vector<Scene*> scenes;

	int currentScene;
public:

	SceneManager();
	virtual ~SceneManager();
	void AddScene();
	Scene& EditScene();
	void SwitchScene(int index);
	void RemoveAllScenes();
	void RenderScene();
	void RenderLights();
};

