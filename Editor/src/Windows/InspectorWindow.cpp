#include "InspectorWindow.h"

#include <Engine/Event/EntityEvents.h>
#include <Engine/Event/Event.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_stdlib.h>
#include <ComponentSystem/Entity.h>
#include <ComponentSystem/Component.h>
#include <ComponentSystem/ComponentRegistry.hpp>

#include <CommonUtilities/Math/Vector.hpp>


InspectorWindow::InspectorWindow(std::vector<std::shared_ptr<Comp::Entity>>& aSelectedEntitiesRef)
	: EditorWindow("Inspector"), mySelectedEntities(aSelectedEntitiesRef)
{
}

void InspectorWindow::Update(float aTimeStep)
{
}

void InspectorWindow::Render()
{
}

void InspectorWindow::UpdateImGui()
{
	if (ImGui::Begin((std::string("Inspector##") + std::to_string(myId)).c_str(), &myOpenFlag))
	{
		//only render if only one entity is selected
		if (mySelectedEntities.size() == 1)
		{
			DrawEntity(mySelectedEntities[0]);

			ImGui::Button("AddComponent");
			if (ImGui::BeginPopupContextItem(std::to_string(mySelectedEntities[0]->GetID()).c_str(), ImGuiPopupFlags_MouseButtonLeft))
			{
				auto& compRegMap = Comp::ComponentRegistry::GetFactoryMap();

				//iterator for-loop with compRegMap
				for (auto it = compRegMap.begin(); it != compRegMap.end(); ++it)
				{
					if (ImGui::MenuItem(it->first.c_str()))
					{
						mySelectedEntities[0]->AddComponent(Comp::ComponentRegistry::Create(it->first));
					}
				}
				ImGui::EndPopup();
			}
		}

	}
	ImGui::End();
}


void InspectorWindow::DrawEntity(std::shared_ptr<Comp::Entity> aEntity)
{
	std::string entityID = std::to_string(aEntity->GetID());
	ImGui::InputText(("Name##" + entityID).c_str(), &const_cast<std::string&>(aEntity->GetName()));

	//Transform
	ImGui::Text("Transform");

	ImGui::DragFloat3(("Position##" + entityID).c_str(), &const_cast<CommonUtilities::Vector3f&>(aEntity->GetTransform().GetPosition()).x);
	ImGui::DragFloat3(("Rotation##" + entityID).c_str(), &const_cast<CommonUtilities::Vector3f&>(aEntity->GetTransform().GetRotation()).x);
	ImGui::DragFloat3(("Scale##" + entityID).c_str(), &const_cast<CommonUtilities::Vector3f&>(aEntity->GetTransform().GetScale()).x, 0.05f);
	aEntity->GetTransform().RecalculateTransform();

	auto& components = aEntity->GetComponents();
	for (int i = 0; i < components.size(); i++)
	{
		auto& parameters = components[i]->GetParameters();

		ImGui::Spacing();
		ImGui::Text(components[i]->GetName().c_str());

		int paramID = 0;
		for (int paramIndex = 0; paramIndex < parameters.size(); paramIndex++)
		{
			//TODO: implement all of these
			//	Int,
			//Float,
			//	String,
			//	Bool,
			//	Vec2,
			//	Vec3,
			//	Vec4,
			//	Color
			auto& param = parameters[paramIndex];
			bool changed = false;
			switch (param.myType)
			{
			case Comp::ParameterType::Int:
			{
				int& value = *reinterpret_cast<int*>(param.myValue);
				changed = ImGui::DragInt((param.myName + "##" + entityID + std::to_string(paramID)).c_str(), &value);
				break;
			}

			case Comp::ParameterType::Float:
			{
				float& value = *reinterpret_cast<float*>(param.myValue);
				changed = ImGui::DragFloat((param.myName + "##" + entityID + std::to_string(paramID)).c_str(), &value);
				break;
			}

			case Comp::ParameterType::String:
			{
				changed = ImGui::InputText((param.myName + "##Param" + entityID + std::to_string(paramID)).c_str(), static_cast<std::string*>(param.myValue));
				break;
			}

			case Comp::ParameterType::Bool:
			{
				bool& value = *reinterpret_cast<bool*>(param.myValue);
				changed = ImGui::Checkbox((param.myName + "##" + entityID + std::to_string(paramID)).c_str(), &value);
				break;
			}
			
			case Comp::ParameterType::Vec2:
			{
				CommonUtilities::Vector2f& value = *reinterpret_cast<CommonUtilities::Vector2f*>(param.myValue);
				changed = ImGui::DragFloat2((param.myName + "##" + entityID + std::to_string(paramID)).c_str(), &value.x);
				break;
			}

			case Comp::ParameterType::Vec3:
			{
				CommonUtilities::Vector3f& value = *reinterpret_cast<CommonUtilities::Vector3f*>(param.myValue);
				changed = ImGui::DragFloat3((param.myName + "##" + entityID + std::to_string(paramID)).c_str(), &value.x);
				break;
			}

			case Comp::ParameterType::Vec4:
			{
				CommonUtilities::Vector4f& value = *reinterpret_cast<CommonUtilities::Vector4f*>(param.myValue);
				changed = ImGui::DragFloat4((param.myName + "##" + entityID + std::to_string(paramID)).c_str(), &value.x);
				break;
			}

			case Comp::ParameterType::Color:
			{
				CommonUtilities::Vector4f& value = *reinterpret_cast<CommonUtilities::Vector4f*>(param.myValue);
				changed = ImGui::ColorEdit4((param.myName + "##" + entityID + std::to_string(paramID)).c_str(), &value.x);
				break;
			}


			}
			if (changed)
			{
				EntityPropertyUpdatedEvent ev;
				aEntity->OnEvent(ev);
			}
			paramID++;
		}
	}

}
