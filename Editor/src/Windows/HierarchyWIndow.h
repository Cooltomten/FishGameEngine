#pragma once
#include "Windows/EditorWindow.h"


class HierarchyWindow : public EditorWindow
{
public:
	HierarchyWindow();
	~HierarchyWindow() override =default;
	// Inherited via EditorWindow
	virtual void Update(float aTimeStep) override;
	virtual void Render() override;
	virtual void UpdateImGui() override;

};
