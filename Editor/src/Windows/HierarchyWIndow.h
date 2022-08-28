#pragma once
#include "Windows/EditorWindow.h"


class HierarchyWindow : public EditorWindow
{
	HierarchyWindow();
	~HierarchyWindow() override =default;
	// Inherited via EditorWindow
	virtual void Update(float aTimeStep) override;
	virtual void Render() override;


};
