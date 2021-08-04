#include "SceneManager.h"

SceneManager::SceneManager()
{
	this->currentScene = 0;
}

SceneManager::~SceneManager()
{
	while (!this->scenes.empty())
	{
		delete this->scenes[(int)this->scenes.size() - 1];
		this->scenes.pop_back();
	}
}

void SceneManager::AddScene()
{
	Scene* newScene = new Scene();
	scenes.push_back(newScene);
	this->currentScene = (int)scenes.size() - 1;
}

Scene& SceneManager::EditScene()
{
	return *this->scenes[this->currentScene];
}

void SceneManager::SwitchScene(int index)
{
	this->currentScene = index;
}

void SceneManager::RemoveAllScenes()
{
	while (!this->scenes.empty())
	{
		delete this->scenes[(int)this->scenes.size() - 1];
		this->scenes.pop_back();
	}
	this->currentScene = 0;
}

void SceneManager::RenderScene()
{
	if (this->scenes.size() > 0)
	{
		this->scenes[currentScene]->Render();
	}
}
