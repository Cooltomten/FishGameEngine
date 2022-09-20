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

	void Entity::OnEvent(FGE::Event& aEvent)
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

	const uint32_t& Entity::GetID() const
	{
		return myID;
	}

	void Entity::SetName(const std::string& aName)
	{
		myName = aName;
	}

	void Entity::SetTag(const std::string& aTag)
	{
		myTag = aTag;
	}

	std::shared_ptr<Entity> Entity::GetParent() 
	{
		return myParent;
	}

	std::vector<std::shared_ptr<Entity>>& Entity::GetChildren()
	{
		return myChildren;
	}

	std::vector<std::shared_ptr<Component>>& Entity::GetComponents()
	{
		return myComponents;
	}
	int Entity::GetRecursiveChildrenCount()
	{
		int count = 0;
		auto children = GetChildren();
		while (children.size() > 0)
		{
			auto child = *children.begin();
			for (auto c : child->GetChildren())
			{
				children.push_back(c);
			}
			children.erase(children.begin());
			count++;
		}
		return count;
	}

	std::vector<std::shared_ptr<Entity>> Entity::GetChildrenRecursive()
	{
		std::vector<std::shared_ptr<Entity>> children;
		auto childrenVec = GetChildren();
		while (childrenVec.size() > 0)
		{
			
			auto child = *childrenVec.begin();
			int index = 1;
			for (auto c : child->GetChildren())
			{
				childrenVec.insert(childrenVec.begin() + index, c);
				index++;
			}
			children.push_back(child);
			childrenVec.erase(childrenVec.begin());
		}
		return children;
	}
	
	std::shared_ptr<Comp::Entity> Entity::GetChild(uint32_t aIndex)
	{
		return myChildren[aIndex];
	}

	void Entity::SetParent(std::shared_ptr<Entity> aParent, int aChildIndex)
	{
		std::shared_ptr<Entity> thisEntity;
		if (aParent)
		{
			if (aChildIndex == -1)
			{
				aChildIndex = aParent->myChildren.size();
				if (aParent == myParent)
				{
					aChildIndex--;
				}
			}
		}
		auto sceneEntities = myParentScene->GetEntities();
		for (int i = 0; i < sceneEntities.size(); i++)
		{
			if (sceneEntities[i]->GetID() == myID)
			{
				thisEntity = sceneEntities[i];
				break;
			}
		}
		if (myParent)
		{

			//Remove as child from old parent
			auto it = std::find(myParent->myChildren.begin(), myParent->myChildren.end(), thisEntity);
			myParent->myTransform.RemoveChild(&myTransform);
			myParent->myChildren.erase(it);

		}
		myParent = aParent;
		if (myParent)
		{
		myTransform.SetParent(&myParent->myTransform);
		}
		else
		{
			myTransform.SetParent(nullptr);

		}

		//in the case a parent is set to null, it means that the entity is a root entity
		if (myParent)
		{
			assert(aChildIndex >= 0 && aChildIndex <= myParent->myChildren.size());
			myParent->myTransform.AddChild(&myTransform);
			auto it = myParent->myChildren.begin() + aChildIndex;
			myParent->myChildren.insert(it, thisEntity);
		}
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

	Transform& Entity::GetTransform()
	{
		return myTransform;
	}

}