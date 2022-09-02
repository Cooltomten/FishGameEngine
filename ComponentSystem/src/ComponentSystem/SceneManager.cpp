#include "SceneManager.h"
#include "ComponentSystem/Entity.h"
#include <nlohmann/json.hpp>
#include <fstream>
namespace Comp
{
	SceneManager* SceneManager::myInstance = nullptr;
	void Comp::SceneManager::Initialize()
	{
		myInstance = new SceneManager();
	}

	SceneManager& Comp::SceneManager::Get()
	{
		return *myInstance;
	}

	std::shared_ptr<Scene> Comp::SceneManager::GetCurrentScene()
	{
		return myInstance->myCurrentScene;
	}

	void Comp::SceneManager::NewScene()
	{
		myCurrentScene = std::make_shared<Scene>();
	}

	void Comp::SceneManager::LoadScene(const std::string& aScenePath)
	{
		//TODO: load scene from file
	}

	void SceneManager::SaveCurrentScene(const std::filesystem::path& aPath)
	{
		nlohmann::json saveFile;

		/*saveFile["Test"] = 10;*/
		auto& jsonEntArr = saveFile["Entities"];
		auto& entities = myCurrentScene->GetEntities();
		for (int entIndex = 0; entIndex < entities.size(); entIndex++)
		{
			auto& entity = entities[entIndex];
			auto& jsonEnt = jsonEntArr[entIndex];
			jsonEnt["Name"] = entity->GetName();
			jsonEnt["Tag"] = entity->GetTag();
			jsonEnt["ID"] = entity->GetID();
			jsonEnt["ParentID"] = entity->GetParent() ? entity->GetParent()->GetID() : 0;

			jsonEnt["Position"] = { entity->GetTransform().GetPosition().x, entity->GetTransform().GetPosition().y, entity->GetTransform().GetPosition().z };
			jsonEnt["Rotation"] = { entity->GetTransform().GetRotation().x, entity->GetTransform().GetRotation().y, entity->GetTransform().GetRotation().z };
			jsonEnt["Scale"] = { entity->GetTransform().GetScale().x, entity->GetTransform().GetScale().y, entity->GetTransform().GetScale().z };


			auto& jsonCompArr = jsonEnt["Components"];
			auto& components = entities[entIndex]->GetComponents();
			for (int compIndex = 0; compIndex < components.size(); compIndex++)
			{
				auto& component = components[compIndex];
				auto& jsonComp = jsonCompArr[compIndex];
				//jsonComp["Type"] = component->GetType();
				//jsonComp["Data"] = component->GetData();
			}
		}

		std::ofstream file(aPath);

		file << saveFile;
	}

}