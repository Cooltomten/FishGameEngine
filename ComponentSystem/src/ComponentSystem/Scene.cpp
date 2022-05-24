#include "CompSysPch.h"
#include "Scene.h"
#include "ComponentSystem/Component.h"

#include "FGE/Event/Event.h"

namespace Comp
{
	void Scene::OnEvent(const FGE::Event& aEvent)
	{
		for (auto& type : myComponentStorage)
		{
			auto& vector = type.second;

			for (size_t i = 0; i < vector.size(); i++)
			{
				vector[i]->OnEvent(aEvent);
			}
		}
	}
	void Scene::OnRuntimeStart()
	{
		for (auto& type : myComponentStorage)
		{
			auto& vector = type.second;
			
			for (size_t i = 0; i < vector.size(); i++)
			{
				vector[i]->Initialize();
			}
		}
	}
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
