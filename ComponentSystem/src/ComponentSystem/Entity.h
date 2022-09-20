#pragma once

#include "ComponentSystem/Transform.h"

#include <cassert>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

namespace FGE
{
	class Event;
}

namespace Comp
{
	class RenderBuffer;
	class Rigidbody;
	class Component;
	class Scene;
	class Entity
	{
	public:
		Entity(std::string aName = "New Entity", uint32_t aID = 0, std::string aTag = "Untagged");
		~Entity();

		bool GetIsActive();
		void Initialize();
		void SetActive(bool aActiveFlag);
		void OnEvent(FGE::Event& aEvent);


		const std::string& GetName() const;
		const std::string& GetTag() const;
		const uint32_t& GetID() const;

		void SetName(const std::string& aName);
		void SetTag(const std::string& aTag);

		std::shared_ptr<Entity> GetParent();
		std::vector<std::shared_ptr<Entity>>& GetChildren();
		std::vector<std::shared_ptr<Component>>& GetComponents();

		/// <summary>
		/// This counts all the children and their children and so on.
		/// if you need to use the return value more than once, save it in a variable as this is expensive.
		/// </summary>
		/// <returns></returns>
		int GetRecursiveChildrenCount();
		
		
		std::vector<std::shared_ptr<Entity>> GetChildrenRecursive();

		std::shared_ptr<Comp::Entity> GetChild(uint32_t aIndex);

		/// <summary>
		/// set child index to -1 to place at the end of the children list
		/// </summary>
		/// <param name="aParent"></param>
		/// <param name="aChildIndex"></param>
		void SetParent(std::shared_ptr<Entity> aParent, int aChildIndex = -1);



		template<class T> T* GetComponent();
		template<class T, typename... TArgs> T* GetOrCreateComponent(TArgs&&... aArgs);
		template<class T> bool HasComponent();
		template<class T> bool RemoveComponent();

		bool AddComponent(std::shared_ptr<Component> aComponent);
		void RemoveComponent(std::shared_ptr<Component> aComponent);

		Transform& GetTransform();

	private:
		friend class Scene;
		std::vector<std::shared_ptr<Component>> myComponents;
		std::unordered_map<std::string, std::shared_ptr<Component>> myComponentMap;

		std::shared_ptr<Entity> myParent;
		std::vector<std::shared_ptr<Entity>> myChildren;

		Scene* myParentScene;

		Transform myTransform;

		std::string myName;
		std::string myTag;
		uint32_t myID;
		bool myIsActiveFlag;

	};

	template<class T>
	inline T* Entity::GetComponent()
	{
		auto it = myComponentMap.find(T::GetFactoryName());
		if (it != myComponentMap.end())
		{
			return std::reinterpret_pointer_cast<T>(it->second).get();
		}
		return nullptr;
	}

	template<class T, typename ...TArgs>
	inline T* Entity::GetOrCreateComponent(TArgs && ...aArgs)
	{
		assert(true && "Not implemented");
		return nullptr;
	}

	template<class T>
	inline bool Entity::HasComponent()
	{
		auto it = myComponentMap.find(T::GetFactoryName());
		if (it != myComponentMap.end())
		{
			return true;
		}
		return false;
	}

	template<class T>
	inline bool Entity::RemoveComponent()
	{
		auto it = myComponentMap.find(T::GetFactoryName());
		if (it == myComponentMap.end())
		{
			return false;
		}



		auto comp = it.second;
		myComponentMap.erase(it);

		//find and erase from myComponents
		for (auto vecIt = myComponents.begin(); vecIt != myComponents.end(); ++vecIt)
		{
			if (*vecIt == comp)
			{
				myComponents.erase(vecIt);
				return true;
			}
		}
		return false;
	}

}