#include "EditorPreferencesWindow.h"
#include <FGE/Core/DX11.h>
#include <FGE/Core/Window.h>
#include "ImGuiUtil.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include "Engine/Event/ApplicationEvents.h"

EditorPreferencesWindow::EditorPreferencesWindow(EditorSettings& aEditorSettings)
	: EditorWindow("Editor Preferences"), myEditorSettings(aEditorSettings)
{

	auto color = myEditorSettings.LoadBackgroundFile(myEditorSettings.myLastLoadedBackground);
	if (!myEditorSettings.myBlendActive)
	{
		FGE::Window::Get().GetDX11().SetClearColor({ color.x, color.y, color.z, color.w });
	}
	myColor = { color.x, color.y, color.z, color.w };
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
			if (!myEditorSettings.myBlendActive)
			{
				FGE::Window::Get().GetDX11().SetClearColor({ myColor.x, myColor.y, myColor.z, myColor.w });
			}

		}



		if (ImGui::Button("Save Preference"))
		{
			std::filesystem::path savePath = ImGuiUtil::GetSaveFilePath("Background Preference\0*.bgp", "bgp");

			if (savePath != "")
			{
				std::ofstream file(savePath);

				nlohmann::json json;

				json["Color"] = FGE::Window::Get().GetDX11().GetClearColor();
				file << json;
				file.close();
				myEditorSettings.myLastLoadedBackground = savePath.string();
			}
		}

		if (ImGui::Button("Load Preference"))
		{
			std::filesystem::path loadPath = ImGuiUtil::GetOpenFilePath("Background Preference\0*.bgp");

			if (loadPath != "")
			{
				std::ifstream file(loadPath);

				nlohmann::json json;
				file >> json;
				file.close();

				auto color = json["Color"];
				myColor = { color[0], color[1] , color[2] ,color[3] };
				if (!myEditorSettings.myBlendActive)
				{
					FGE::Window::Get().GetDX11().SetClearColor(color);
				}
				myEditorSettings.myLastLoadedBackground = loadPath.string();
			}
		}

		if (ImGui::Button("Set as Active background"))
		{
			myEditorSettings.myBlendActive = false;
			FGE::Window::Get().GetDX11().SetClearColor({ myColor.x, myColor.y, myColor.z, myColor.w });
		}


	}
	ImGui::End();
}

void EditorPreferencesWindow::OnEvent(FGE::Event& aEvent)
{
	FGE::EventDispatcher dispatcher(aEvent);

}
