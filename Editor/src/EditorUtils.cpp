#include "EditorUtils.h"

#include "Firefly/Application/Window.h"
#include "Firefly/Application/Application.h"

#include "imgui/imgui_internal.h"
#include "imgui/imgui.h"

#include <commdlg.h>
#include <filesystem>
#include <Windows.h>

namespace EditorUtils
{

	std::filesystem::path GetSaveFilePath(const char* aFilter, const char* fileEnding)
	{
		std::string fileEndingWithDot = "." + std::string(fileEnding);
		OPENFILENAMEA ofn = {};
		char szFileName[MAX_PATH] = "";

		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = Firefly::Application::Get().GetWindow()->GetHandle();
		ofn.lpstrFilter = aFilter;
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
		ofn.lpstrDefExt = fileEnding;

		GetSaveFileNameA(&ofn);

		std::filesystem::path path = szFileName;

		if (path != "")
		{
			if (path.has_extension())
			{

				if (path.extension() != fileEndingWithDot)
				{
					path += fileEnding;
				}
			}
			else
			{
				path += fileEndingWithDot;
			}
		}


		return path;
	}

	std::filesystem::path GetOpenFilePath(const char* aFilter)
	{
		OPENFILENAMEA ofn = {};
		char szFileName[MAX_PATH] = "";

		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = Firefly::Application::Get().GetWindow()->GetHandle();
		ofn.lpstrFilter = aFilter;
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;

		GetOpenFileNameA(&ofn);

		return szFileName;
	}

	void SetImGuiPayloadActive(bool aActive)
	{
		auto& context = *ImGui::GetCurrentContext();
	
		context.DragDropActive = aActive;
	}

}