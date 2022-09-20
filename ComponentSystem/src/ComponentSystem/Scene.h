#pragma once
#include <memory>
#include <string>
#include <unordered_map>
namespace FGE
{
	class Event;
}
namespace Comp
{
	class Component;
	class Entity;
	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;

		void OnEvent( FGE::Event& aEvent);

		void OnRuntimeStart();

		void AddEntity(std::shared_ptr<Entity> aEntity);
		std::shared_ptr<Entity> InstatiateEntity();

		void DeleteEntity(std::shared_ptr<Entity> aEntity);
		std::vector<std::shared_ptr<Entity>>& GetEntities();
		
		std::shared_ptr<Entity> GetEntityByID(uint32_t aID);

	private:

		friend class Entity;
		std::vector<std::shared_ptr<Entity>> myEntities;

		static uint32_t myCurrentID;
	};
}