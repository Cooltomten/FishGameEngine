#include "Entity.h"
#include "ComponentSystem/Component.h"
#include "ComponentSystem/Scene.h"

namespace Comp
{
	Entity::Entity(std::string aName, uint32_t aID, std::string aTag)
	{
		myParentScene = nullptr;
		myParent = nullptr;
		myName = aName;
		myTag = aTag;
		myID = aID;
	}

	Entity::~Entity()
	{
	}

	bool Entity::GetIsActive()
	{
		return myIsActiveFlag;
	}

	void Entity::Initialize()
	{
		for (int i = 0; i < myComponents.size(); i++)
		{
			myComponents[i]->Initialize();
		}
	}

	void Entity::SetActive(bool aActiveFlag)
	{
		myIsActiveFlag = aActiveFlag;
	}

	void Entity::OnEvent(const FGE::Event& aEvent)
	{
		for (int i = 0; i < myComponents.size(); i++)
		{
			myComponents[i]->OnEvent(aEvent);
		}
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