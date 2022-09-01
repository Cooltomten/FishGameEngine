#pragma once
#include "Windows/EditorWindow.h"
#include <memory>
#include <ComponentSystem/Entity.h>



class HierarchyWindow : public EditorWindow
{
public:
	HierarchyWindow(std::vector<std::shared_ptr<Comp::Entity>>& aSelectedEntitiesRef);
	~HierarchyWindow() override =default;
	
	virtual void Update(float aTimeStep) override;
	virtual void Render() override;
	virtual void UpdateImGui() override;
private:
	void DrawEntity(std::shared_ptr<Comp::Entity> aEntity);
	std::vector<std::shared_ptr<Comp::Entity>>& mySelectedEntities;

};
