#include "HierarchyWindow.h"
#include <ImGui/imgui.h>

HierarchyWindow::HierarchyWindow()
	: EditorWindow("Hierarchy")
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
	if (ImGui::Begin((std::string("Hierarchy#") + std::to_string(myId)).c_str(), &myOpenFlag))
	{
		ImGui::Text("Hello World");
	}
	ImGui::End();
}
