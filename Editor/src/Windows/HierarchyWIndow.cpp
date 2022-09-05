#include "HierarchyWindow.h"
#include <ImGui/imgui.h>
#include <ComponentSystem/SceneManager.h>
#include <ComponentSystem/Scene.h>
#include <ComponentSystem/Entity.h>

HierarchyWindow::HierarchyWindow(std::vector<std::shared_ptr<Comp::Entity>>& aSelectedEntitiesRef)
	: EditorWindow("Hierarchy"), mySelectedEntities(aSelectedEntitiesRef)
{
}

void HierarchyWindow::Update(float aTimeStep)
{
}

void HierarchyWindow::Render()
{
}

void HierarchyWindow::UpdateImGui()
{
	if (ImGui::Begin((std::string("Hierarchy##") + std::to_string(myId)).c_str(), &myOpenFlag))
	{
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Create Empty"))
			{
				Comp::SceneManager::GetCurrentScene()->InstatiateEntity();
			}
			ImGui::EndPopup();
		}
		auto currentScene = Comp::SceneManager::GetCurrentScene();
		if (currentScene)
		{
			for (auto& entity : currentScene->GetEntities())
			{
				if (entity->GetParent() == nullptr)
				{
					DrawEntity(entity);
				}
			}
		}
	}
	ImGui::End();
}

void HierarchyWindow::DrawEntity(std::shared_ptr<Comp::Entity> aEntity)
{
	int flags = 0;
	auto& children = aEntity->GetChildren();
	if (children.size() <= 0)
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	auto it = std::find_if(mySelectedEntities.begin(), mySelectedEntities.end(),
		[aEntity](std::shared_ptr<Comp::Entity>& aEntityToCheck) { return aEntityToCheck->GetID() == aEntity->GetID(); });
	if (it != mySelectedEntities.end())
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	
	if (ImGui::TreeNodeEx((aEntity->GetName() + "##" + std::to_string(aEntity->GetID())).c_str(), flags))
	{
		if (ImGui::IsItemClicked())
		{
			mySelectedEntities.clear();
			mySelectedEntities.push_back(aEntity);
		}
		for (auto& child : children)
		{
			DrawEntity(child);
		}
		ImGui::TreePop();
	}
}
