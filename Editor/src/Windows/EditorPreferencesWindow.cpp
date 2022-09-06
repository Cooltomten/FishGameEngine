#include "EditorPreferencesWindow.h"
#include <FGE/Core/DX11.h>
#include <FGE/Core/Window.h>

EditorPreferencesWindow::EditorPreferencesWindow()
	: EditorWindow("Editor Preferences")
{
	auto color = FGE::Window::Get().GetDX11().GetClearColor();
	myColor = ImVec4();
}

void EditorPreferencesWindow::Update(float aTimeStep)
{
}

void EditorPreferencesWindow::Render()
{
}

void EditorPreferencesWindow::UpdateImGui()
{
	if (ImGui::Begin((std::string("Editor Preferences##") + std::to_string(myId)).c_str(), &myOpenFlag))
	{
		if (ImGui::ColorEdit4("Background Color", &myColor.x))
		{
			FGE::Window::Get().GetDX11().SetClearColor({ myColor.x, myColor.y, myColor.z, myColor.w });
		}
	}
	ImGui::End();
}
