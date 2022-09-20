#include "HierarchyWindow.h"
#include <ComponentSystem/SceneManager.h>
#include <ComponentSystem/Scene.h>
#include <ComponentSystem/Entity.h>

#include "UndoSystem/UndoHandler.h"
#include "UndoSystem/Commands/MoveInHierarchyCommand.h"

#include "ImGuiUtil.h"

HierarchyWindow::HierarchyWindow(std::vector<std::shared_ptr<Comp::Entity>>& aSelectedEntitiesRef)
	: EditorWindow("Hierarchy"), mySelectedEntities(aSelectedEntitiesRef)
{
	myBetweenIndex = -1;
	myDragging = false;
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
		if (ImGui::Button("Undo"))
		{
			myUndoHandler->Undo();
		}
		ImGui::SameLine();
		if (ImGui::Button("Redo"))
		{
			myUndoHandler->Redo();
		}

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Create Empty"))
			{
				Comp::SceneManager::GetCurrentScene()->InstatiateEntity();
			}
			ImGui::EndPopup();
		}

		auto currentScene = Comp::SceneManager::GetCurrentScene();
		myDragging = false;
		if (currentScene)
		{
			ImVec2 cursorPos = ImGui::GetCursorPos();
			ImVec2 elementSize = ImGui::GetItemRectSize();
			elementSize.x -= ImGui::GetStyle().FramePadding.x;
			elementSize.y = ImGui::GetStyle().FramePadding.y;
			cursorPos.y -= ImGui::GetStyle().FramePadding.y;
			ImVec2 windowPos = ImGui::GetWindowPos();
			myBetweenRects.emplace_back(ImVec2(windowPos.x + cursorPos.x, windowPos.y + cursorPos.y),
				ImVec2(windowPos.x + cursorPos.x + elementSize.x, windowPos.y + cursorPos.y + elementSize.y), false);
			auto entities = currentScene->GetEntities();
			for (int i = 0; i < entities.size(); i++)
			{
				if (entities[i]->GetParent() == nullptr)
				{
					DrawEntity(entities[i]);
				}
			}
		}
		if (myDragging)
		{
			myBetweenIndex = -1;
			for (int i = 0; i < myBetweenRects.size(); i++)
			{
				auto& rect = myBetweenRects[i];
				if (ImGui::IsMouseHoveringRect(rect.Min, rect.Max))
				{
					myBetweenIndex = i;
				}
			}
			if (myBetweenIndex == -1)
			{
				if (ImGui::IsMouseHoveringRect(ImGui::GetWindowContentRegionMin(), ImGui::GetWindowContentRegionMax()) && ImGui::GetMousePos().y > myBetweenRects.back().Max.y)
				{
					myBetweenIndex = myBetweenRects.size() - 1;
				}
			}
		}
		if (myBetweenIndex != -1)
		{
			ImGui::GetWindowDrawList()->AddRectFilled(myBetweenRects[myBetweenIndex].Min, myBetweenRects[myBetweenIndex].Max, ImGui::GetColorU32(ImGuiCol_DragDropTarget));
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			{
				ImGuiUtil::SetImGuiPayloadActive(true);
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY", ImGuiDragDropFlags_AcceptBeforeDelivery))
				{
					std::shared_ptr<Comp::Entity> entity = *(std::shared_ptr<Comp::Entity>*)payload->Data;
					auto& sceneEntities = Comp::SceneManager::GetCurrentScene()->GetEntities();

					auto it = std::find(sceneEntities.begin(), sceneEntities.end(), entity);
					int itIndex = it - sceneEntities.begin();

					int insertIndex = myBetweenIndex;

					//account for closed entities
					for (int i = 0; i < myBetweenIndex - 1; i++)
					{
						if (!myBetweenRects[i + 1].OpenFlag)
						{
							insertIndex += sceneEntities[i]->GetRecursiveChildrenCount();
						}
					}

					//find parent
					std::shared_ptr<Comp::Entity> parent;
					if (myBetweenIndex != 0 && insertIndex < sceneEntities.size())
					{
						parent = sceneEntities[insertIndex]->GetParent();
					}

					//check if parent is valid
					bool invalid = false;
					if (parent)
					{
						auto testParent = parent;
						while (testParent)
						{
							if (testParent == entity)
							{
								invalid = true;
								break;
							}
							testParent = testParent->GetParent();
						}
					}

					//Move entity and set parent
					if (!invalid)
					{
						int indexToRemoveAt = it - sceneEntities.begin();

						//find what index the child should be at
						int childIndex = -1;
						if (parent)
						{
							auto& childrenOfParent = parent->GetChildren();
							auto insertEntityInChildrenIt = std::find(childrenOfParent.begin(), childrenOfParent.end(), sceneEntities[insertIndex]);
							childIndex = insertEntityInChildrenIt - childrenOfParent.begin();
							if (entity->GetParent() == parent)
							{
								int currentChildIndex = std::find(parent->GetChildren().begin(), parent->GetChildren().end(), entity) - parent->GetChildren().begin();
								if (childIndex > currentChildIndex)
								{
									childIndex--;
								}
							}
						}
						//Move Entity
						myUndoHandler->ExecuteAndAdd(
							std::make_shared<MoveInHierarchyCommand>(sceneEntities, indexToRemoveAt, insertIndex,
								parent, childIndex));

					}
					//SORT CHILDREN
				}
				ImGuiUtil::SetImGuiPayloadActive(false);
				myBetweenIndex = -1;
			}
		}
		myDragging = false;
		myBetweenRects.clear();
	}
	ImGui::End();
}

void HierarchyWindow::DrawEntity(std::shared_ptr<Comp::Entity> aEntity)
{
	int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_DefaultOpen;

	//if Entity has no children, dont draw an aroow to the left of the name
	auto& children = aEntity->GetChildren();
	if (children.size() <= 0)
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}

	//if Entity is selected, draw it selected
	auto it = std::find_if(mySelectedEntities.begin(), mySelectedEntities.end(),
		[aEntity](std::shared_ptr<Comp::Entity>& aEntityToCheck) { return aEntityToCheck->GetID() == aEntity->GetID(); });
	if (it != mySelectedEntities.end())
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	//DEBUG
	auto& sceneEntities = Comp::SceneManager::GetCurrentScene()->GetEntities();
	auto itEnt = std::find(sceneEntities.begin(), sceneEntities.end(), aEntity);
	int itEntIndex = itEnt - sceneEntities.begin();
	auto id = aEntity->GetName() + std::to_string(itEntIndex) + "##HierarchyEntity" + std::to_string(aEntity->GetID());

	bool treeBool = ImGui::TreeNodeEx(id.c_str(), flags);
	ImVec2 cursorPos = ImGui::GetCursorPos();
	ImVec2 elementSize = ImGui::GetItemRectSize();
	elementSize.x -= ImGui::GetStyle().FramePadding.x;
	elementSize.y = ImGui::GetStyle().FramePadding.y;
	cursorPos.y -= ImGui::GetStyle().FramePadding.y;
	ImVec2 windowPos = ImGui::GetWindowPos();
	myBetweenRects.emplace_back(ImVec2(windowPos.x + cursorPos.x, windowPos.y + cursorPos.y),
		ImVec2(windowPos.x + cursorPos.x + elementSize.x, windowPos.y + cursorPos.y + elementSize.y),
		treeBool && children.size() > 0);
	// Track the in-between size for this tree node
	if (ImGui::IsItemClicked())
	{
		mySelectedEntities.clear();
		mySelectedEntities.push_back(aEntity);
	}
	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("ENTITY", &aEntity, sizeof(std::shared_ptr<Comp::Entity>));
		myDragging = true;
		ImGui::EndDragDropSource();
	}
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY"))
		{
			std::shared_ptr<Comp::Entity> child = *(std::shared_ptr<Comp::Entity>*)payload->Data;
			auto parent = aEntity;

			auto& sceneEntities = Comp::SceneManager::GetCurrentScene()->GetEntities();

			auto testParent = parent->GetParent();
			bool invalid = false;
			while (testParent)
			{
				if (testParent == child)
				{
					invalid = true;
					break;
				}
				testParent = testParent->GetParent();
			}
			if (!invalid)
			{

				auto childIt = std::find(sceneEntities.begin(), sceneEntities.end(), child);
				int childItIndex = childIt - sceneEntities.begin();
				//sceneEntities.erase(childIt);

				auto parentIt = std::find(sceneEntities.begin(), sceneEntities.end(), parent);
				int parentItIndex = parentIt - sceneEntities.begin();

				int insertIndex = parentItIndex  + 1 + parent->GetRecursiveChildrenCount();
				int childIndex = -1;

				auto& childrenOfParent = parent->GetChildren();
				childIndex = childrenOfParent.size();
				if (child->GetParent() == parent)
				{
					childIndex--;
				}


				//Move Entity
				myUndoHandler->ExecuteAndAdd(
					std::make_shared<MoveInHierarchyCommand>(sceneEntities, childItIndex, insertIndex,
						parent,childIndex));
			}
		}
		ImGui::EndDragDropTarget();
	}

	if (ImGui::BeginPopupContextItem())
	{
		mySelectedEntities.clear();
		mySelectedEntities.push_back(aEntity);
		if (ImGui::MenuItem("Delete"))
		{
			Comp::SceneManager::GetCurrentScene()->DeleteEntity(aEntity);
			ImGui::EndPopup();
			ImGui::TreePop();
			mySelectedEntities.clear();
			return;
		}
		ImGui::EndPopup();
	}
	if (treeBool)
	{
		ImGui::TreePush(id.c_str());


		for (int i = 0; i < children.size(); ++i)
		{
			DrawEntity(children[i]);
		}




		ImGui::TreePop();
	}




}