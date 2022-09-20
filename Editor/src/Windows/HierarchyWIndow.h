#pragma once
#include "Windows/EditorWindow.h"
#include <memory>
#include <ComponentSystem/Entity.h>

#include "ImGui/imgui.h"



class HierarchyWindow : public EditorWindow
{
public:
	HierarchyWindow(std::vector<std::shared_ptr<Comp::Entity>>& aSelectedEntitiesRef);
	~HierarchyWindow() override =default;
	
	virtual void Update(float aTimeStep) override;
	virtual void Render() override;
	virtual void UpdateImGui() override;

	static std::string GetWindowName() { return "Hierarchy"; }
	
private:
	void DrawEntity(std::shared_ptr<Comp::Entity> aEntity);
	bool myDragging;

	int myBetweenIndex = 0;

	std::vector<std::shared_ptr<Comp::Entity>>& mySelectedEntities;

	struct ImRect
	{
		ImRect() = default;
		ImRect(ImVec2 aMin, ImVec2 aMax, bool aOpenFlag)
			: Min(aMin), Max(aMax), OpenFlag(aOpenFlag)
		{
		}

		ImVec2 Min;
		ImVec2 Max;

		bool OpenFlag;
	};

	std::vector<ImRect> myBetweenRects;

};
