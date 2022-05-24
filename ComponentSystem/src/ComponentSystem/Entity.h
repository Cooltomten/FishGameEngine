#pragma once

#include "ComponentSystem/Transform.h"
#include "ComponentSystem/Scene.h"

#include <cassert>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
namespace Comp
{
	class RenderBuffer;
	class Rigidbody;
	class Component;
	class Scene;
	class Entity
	{
	public:
		Entity(Scene* aParentScene, std::string aName, uint32_t aID = 0, std::string aTag = "Untagged");
		~Entity();

		bool GetIsActive();
		void SetActive(bool aActiveFlag);

		const std::string& GetName() const;
		const std::string& GetTag() const;

		template<class T> T* GetComponent();
		template<class T, typename... TArgs> T* GetOrCreateComponent(TArgs&&... aArgs);
		template<class T> bool HasComponent();
		template<class T> bool RemoveComponent();

		bool AddComponent(std::shared_ptr<Component> aComponent);
		void RemoveComponent(std::shared_ptr<Component> aComponent);

	private:
		std::vector<std::shared_ptr<Component>> myComponents;
		std::unordered_map<std::string, std::shared_ptr<Component>> myComponentMap;
		Entity* myParent;
		std::vector<Entity*> myChildren;
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

		myParentScene->UnregisterComponent(it.second);

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