#include "CompSysPch.h"
#include "Scene.h"
#include "ComponentSystem/Component.h"

#include "ComponentSystem/Entity.h"

#include <Engine/Event/Event.h>

namespace Comp
{
	uint32_t Scene::myCurrentID = 1; // ID 0 is reserved for not having a parent
	void Scene::OnEvent( FGE::Event& aEvent)
	{

			for (size_t i = 0; i < myEntities.size(); i++)
			{
				myEntities[i]->OnEvent(aEvent);
			}
	}
	void Scene::OnRuntimeStart()
	{
		for (size_t i = 0; i < myEntities.size(); i++)
		{
			myEntities[i]->Initialize();
		}
	}
	void Scene::AddEntity(std::shared_ptr<Entity> aEntity)
	{
		aEntity->myParentScene = this;
		myEntities.push_back(aEntity);
	}
	
	void Scene::DeleteEntity(std::shared_ptr<Entity> aEntity)
	{
		auto it = std::find(myEntities.begin(), myEntities.end(), aEntity);
		for (auto child : (*it._Ptr)->GetChildren())
		{
			DeleteEntity(child);
		}

		myEntities.erase(it);
	}

	std::vector<std::shared_ptr<Entity>>& Scene::GetEntities()
	{
		return myEntities;
	}

	std::shared_ptr<Entity> Scene::GetEntityByID(uint32_t aID)
	{
		for (size_t i = 0; i < myEntities.size(); i++)
		{
			if (myEntities[i]->GetID() == aID)
			{
				return myEntities[i];
			}
		}
		return std::shared_ptr<Entity>();
	}

	 std::shared_ptr<Entity> Scene::InstatiateEntity()
	 {
		 std::shared_ptr<Entity> entity = std::make_shared<Entity>("New Entity", myCurrentID++);
		 AddEntity(entity);
		 return entity;
	 }

}
