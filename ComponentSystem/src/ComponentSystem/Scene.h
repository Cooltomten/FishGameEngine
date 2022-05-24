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

		void OnEvent(const FGE::Event& aEvent);

		void OnRuntimeStart();

	private:
		void RegisterComponent(const std::string& aName, std::shared_ptr<Component> aComponent);
		void UnregisterComponent(std::shared_ptr<Component> aComponent);

		friend class Entity;

		std::unordered_map<std::string, std::vector<std::shared_ptr<Component>>> myComponentStorage;
	};
}