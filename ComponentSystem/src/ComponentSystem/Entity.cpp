#include "Entity.h"
#include "ComponentSystem/Component.h"
#include "ComponentSystem/Scene.h"

namespace Comp
{
	Entity::Entity(Scene* aParentScene, std::string aName, uint32_t aID, std::string aTag)
	{
		myName = aName;
		myTag = aTag;
		myID = aID;
		myParentScene = aParentScene;
	}

	Entity::~Entity()
	{
	}

	bool Entity::GetIsActive()
	{
		return myIsActiveFlag;
	}

	void Entity::SetActive(bool aActiveFlag)
	{
		myIsActiveFlag = aActiveFlag;
	}

	const std::string& Entity::GetName() const
	{
		return myName;
	}

	const std::string& Entity::GetTag() const
	{
		return myTag;
	}

	bool Entity::AddComponent(std::shared_ptr<Component> aComponent)
	{
		std::string name = aComponent->GetName();

		auto it = myComponentMap.find(name);
		if (it == myComponentMap.end())
		{
			aComponent->myEntity = this;

			if (myParentScene)
			{
				myParentScene->RegisterComponent(name, aComponent);
			}

			myComponents.push_back(aComponent);
			myComponentMap[name] = aComponent;

			return true;
		}
		return false;
	}

	void Entity::RemoveComponent(std::shared_ptr<Component> aComponent)
	{
		std::string name = aComponent->GetName();

		auto it = myComponentMap.find(name);
		if (it != myComponentMap.end())
		{
			if (myParentScene)
			{
				myParentScene->UnregisterComponent(aComponent);
			}

			for (auto vecIt = myComponents.begin(); vecIt != myComponents.end(); ++vecIt)
			{
				if (*vecIt == it->second)
				{
					myComponents.erase(vecIt);
					break;
				}
			}
			
			myComponentMap.erase(name);
		}
	}

}