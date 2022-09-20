#include "SceneManager.h"
#include "ComponentSystem/Entity.h"
#include "ComponentSystem/ComponentRegistry.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <ComponentSystem/Component.h>
#include <CommonUtilities/Math/Vector2.hpp>
#include <CommonUtilities/Math/Vector3.hpp>
#include <CommonUtilities/Math/Vector4.hpp>

#include <Engine/Event/EntityEvents.h>
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



	void SerializeParameter(const Parameter& aParameter, nlohmann::json& aJson)
	{
		aJson["Name"] = aParameter.myName;

		aJson["Type"] = ParameterTypeToString(aParameter.myType);

		switch (aParameter.myType)
		{
		case Comp::ParameterType::Int:
			aJson["Value"] = *static_cast<int*>(aParameter.myValue);
			break;

		case Comp::ParameterType::Float:
			aJson["Value"] = *static_cast<float*>(aParameter.myValue);
			break;

		case Comp::ParameterType::String:
			aJson["Value"] = *static_cast<std::string*>(aParameter.myValue);
			break;

		case Comp::ParameterType::Bool:
			aJson["Value"] = *static_cast<bool*>(aParameter.myValue);
			break;

		case Comp::ParameterType::Vec2:
		{
			CommonUtilities::Vector2f vec2 = *static_cast<CommonUtilities::Vector2f*>(aParameter.myValue);
			aJson["Value"] = { vec2.x, vec2.y };
		}
		break;

		case Comp::ParameterType::Vec3:
		{
			CommonUtilities::Vector3f vec3 = *static_cast<CommonUtilities::Vector3f*>(aParameter.myValue);
			aJson["Value"] = { vec3.x, vec3.y, vec3.z };
		}
		break;

		case Comp::ParameterType::Vec4:
		{
			CommonUtilities::Vector4f vec4 = *static_cast<CommonUtilities::Vector4f*>(aParameter.myValue);
			aJson["Value"] = { vec4.x, vec4.y, vec4.z, vec4.w };
		}
		break;

		case Comp::ParameterType::Color:
		{
			CommonUtilities::Vector4f color = *static_cast<CommonUtilities::Vector4f*>(aParameter.myValue);
			aJson["Value"] = { color.x, color.y, color.z, color.w };
		}
		break;

		}

	}

	void SceneManager::SaveCurrentScene(const std::filesystem::path& aPath)
	{
		myCurrentScenePath = aPath;
		

		nlohmann::json saveFile;
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
				jsonComp["Name"] = component->GetName();

				auto& jsonParamArray = jsonComp["Parameters"];
				auto& params = component->GetParameters();
				for (int paramIndex = 0; paramIndex < params.size(); paramIndex++)
				{
					auto& param = component->GetParameters()[paramIndex];
					SerializeParameter(param, jsonParamArray[paramIndex]);
				}

			}
		}

		std::ofstream file(aPath);

		file << saveFile;
		file.close();
	}

	const std::filesystem::path& SceneManager::GetCurrentScenePath()
	{
		return myCurrentScenePath;
	}

	void DeserialzieParameter(nlohmann::json& aJson, Parameter& aParameter)
	{
		ParameterType type = StringToParameterType(aJson["Type"]);

		switch (type)
		{
		case Comp::ParameterType::Int:
			*static_cast<int*>(aParameter.myValue) = aJson["Value"];
			break;

		case Comp::ParameterType::Float:
			*static_cast<float*>(aParameter.myValue) = aJson["Value"];
			break;

		case Comp::ParameterType::String:
			*static_cast<std::string*>(aParameter.myValue) = aJson["Value"];
			break;

		case Comp::ParameterType::Bool:
			*static_cast<bool*>(aParameter.myValue) = aJson["Value"];
			break;

		case Comp::ParameterType::Vec2:
		{
			auto& vec2 = aJson["Value"];
			*static_cast<CommonUtilities::Vector2f*>(aParameter.myValue) = { vec2[0], vec2[1] };
		}
		break;

		case Comp::ParameterType::Vec3:
		{
			auto& vec3 = aJson["Value"];
			*static_cast<CommonUtilities::Vector3f*>(aParameter.myValue) = { vec3[0], vec3[1], vec3[2] };
		}
		break;

		case Comp::ParameterType::Vec4:
		{
			auto& vec4 = aJson["Value"];
			*static_cast<CommonUtilities::Vector4f*>(aParameter.myValue) = { vec4[0], vec4[1], vec4[2], vec4[3] };
		}
		break;

		case Comp::ParameterType::Color:
		{
			auto& color = aJson["Value"];
			*static_cast<CommonUtilities::Vector4f*>(aParameter.myValue) = { color[0], color[1], color[2], color[3] };
		}
		break;
		}
	}

	void Comp::SceneManager::LoadScene(const std::string& aScenePath)
	{
		myCurrentScenePath = aScenePath;
		
		std::ifstream file(aScenePath);
		nlohmann::json json;
		file >> json;
		file.close();

		NewScene();
		std::vector<int> parentIDs;
		auto& jsonEntArr = json["Entities"];
		for (int entIndex = 0; entIndex < jsonEntArr.size(); entIndex++)
		{
			auto& jsonEnt = jsonEntArr[entIndex];
			auto entity = myCurrentScene->InstatiateEntity();
			//std::make_shared<Entity>(jsonEnt["Name"], jsonEnt["ID"], jsonEnt["Tag"]);
			entity->SetName(jsonEnt["Name"]);
			entity->SetTag(jsonEnt["Tag"]);
			const_cast<uint32_t&> (entity->GetID()) = jsonEnt["ID"];

			parentIDs.push_back(jsonEnt["ParentID"]);

			auto& jsonPos = jsonEnt["Position"];
			entity->GetTransform().SetPosition(CommonUtilities::Vector3f(jsonPos[0], jsonPos[1], jsonPos[2]));

			auto& jsonRot = jsonEnt["Rotation"];
			entity->GetTransform().SetRotation(CommonUtilities::Vector3f(jsonRot[0], jsonRot[1], jsonRot[2]));

			auto& jsonScale = jsonEnt["Scale"];
			entity->GetTransform().SetScale(CommonUtilities::Vector3f(jsonScale[0], jsonScale[1], jsonScale[2]));

			auto& jsonCompArr = jsonEnt["Components"];

			for (int compIndex = 0; compIndex < jsonCompArr.size(); compIndex++)
			{
				auto& jsonComp = jsonCompArr[compIndex];
				auto component = ComponentRegistry::Create(jsonComp["Name"]);

				auto& params = component->GetParameters();
				auto& jsonParamArr = jsonComp["Parameters"];
				for (int paramIndex = 0; paramIndex < jsonParamArr.size(); paramIndex++)
				{
					auto& jsonParam = jsonParamArr[paramIndex];
					DeserialzieParameter(jsonParam, const_cast<Parameter&>(params[paramIndex]));
				}

				entity->AddComponent(component);
			}
			EntityPropertyUpdatedEvent ev;
			entity->OnEvent(ev);
		}
		// has to loop again to set parent
		for (int i = 0; i < parentIDs.size(); i++)
		{
			int parentId = parentIDs[i];
			if (parentId != -1)
			{
				auto parent = myCurrentScene->GetEntityByID(parentId);
				auto child = myCurrentScene->GetEntities()[i];
				child->SetParent(parent);
			}
		}

		myCurrentScene->OnRuntimeStart();



	}



}