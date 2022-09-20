#include "ImGuiUtil.h"
#include <FGE/Core/Window.h>

#include "imgui/imgui_internal.h"

#include <commdlg.h>
#include <filesystem>
#include <WinUser.h>
#include <Windows.h>

#include <iostream>

namespace ImGuiUtil
{

	std::filesystem::path ImGuiUtil::GetSaveFilePath(const char* aFilter, const char* fileEnding)
	{
		std::string fileEndingWithDot = "." + std::string(fileEnding);
		OPENFILENAMEA ofn = {};
		char szFileName[MAX_PATH] = "";

		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = FGE::Window::Get().GetHandle();
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

			std::filesystem::path currPath = std::filesystem::current_path();
			if (path.string().compare(0, currPath.string().size(), currPath.string()) == 0)
			{
				path = path.string().substr(currPath.string().size() + 1);
			}
			else
			{
				std::cout << "ERROR: Cannot save file at path : " << path << std::endl;
				std::cout << "Message: Cannot save file in directory outside working directory." << std::endl;
				return "";
			}
		}

		return path;
	}

	std::filesystem::path ImGuiUtil::GetOpenFilePath(const char* aFilter)
	{
		OPENFILENAMEA ofn = {};
		char szFileName[MAX_PATH] = "";

		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = FGE::Window::Get().GetHandle();
		ofn.lpstrFilter = aFilter;
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;

		GetOpenFileNameA(&ofn);
		std::filesystem::path path = szFileName;

		std::filesystem::path currPath = std::filesystem::current_path();
		if (path.string().compare(0, currPath.string().size(), currPath.string()) == 0)
		{
			path = path.string().substr(currPath.string().size() + 1);
		}
		else
		{
			std::cout << "ERROR: Cannot Load file at path : " << path << std::endl;
			std::cout << "Message: Cannot Load file in directory outside working directory." << std::endl;
			return "";
		}

		return path;
	}

	void SetImGuiPayloadActive(bool aActive)
	{
		auto& context = *ImGui::GetCurrentContext();

		context.DragDropActive = aActive;
	}

}