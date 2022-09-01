#include "SceneManager.h"

namespace Comp
{
	SceneManager* SceneManager::myInstance = nullptr;
	void Comp::SceneManager::Initialize()
	{
		myInstance = new SceneManager();
	}

	SceneManager& Comp::SceneManager::Get()
	{
		return *myInstance;
	}

	std::shared_ptr<Scene> Comp::SceneManager::GetCurrentScene()
	{
		return myInstance->myCurrentScene;
	}

	void Comp::SceneManager::NewScene()
	{
		myCurrentScene = std::make_shared<Scene>();
	}

	void Comp::SceneManager::LoadScene(const std::string& aScenePath)
	{
		//TODO: load scene from file
	}

}