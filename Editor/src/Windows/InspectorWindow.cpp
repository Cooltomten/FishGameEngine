#include "InspectorWindow.h"

#include <Engine/Event/EntityEvents.h>
#include <Engine/Event/Event.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_stdlib.h>
#include <ComponentSystem/Entity.h>
#include <ComponentSystem/Component.h>


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
			case Comp::ParameterType::String:
				changed = ImGui::InputText((param.myName + "##Param" + std::to_string(paramID)).c_str(), static_cast<std::string*>(param.myValue));
				break;
				
				
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
