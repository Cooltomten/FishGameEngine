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

	private:

		friend class Entity;
		std::vector<std::shared_ptr<Entity>> myEntities;
	};
}