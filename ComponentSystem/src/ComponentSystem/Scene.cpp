#include "CompSysPch.h"
#include "Scene.h"

namespace Comp
{
	void Scene::RegisterComponent(const std::string& aName, std::shared_ptr<Component> aComponent)
	{
		myComponentStorage[aName].emplace_back( aComponent);
	}
	void Scene::UnregisterComponent(std::shared_ptr<Component> aComponent)
	{
		for (auto& component : myComponentStorage)
		{
			auto it = std::find(component.second.begin(), component.second.end(), aComponent);
			if (it != component.second.end())
			{
				component.second.erase(it);
				break;
			}
		}
	}
}
