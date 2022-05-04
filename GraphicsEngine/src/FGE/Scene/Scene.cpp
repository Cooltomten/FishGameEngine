#include "GraphicsEngine.pch.h"
#include "Scene.h"

namespace FGE
{
	void Scene::AddEntity(std::shared_ptr<Entity> aEntity)
	{
		myEntities.push_back(aEntity);
	}

	void Scene::RemoveEntity(std::shared_ptr<Entity> aEntity)
	{
		myEntities.erase(std::find(myEntities.begin(), myEntities.end(), aEntity));
	}
	std::vector<std::shared_ptr<Entity>> Scene::GetEntities()
	{
		return myEntities;
	}
	void Scene::SetMainCamera(std::shared_ptr<Camera> aCamera)
	{
		myMainCamera = aCamera;
	}
	std::shared_ptr<Camera> Scene::GetMainCamera()
	{
		return myMainCamera;
	}
}