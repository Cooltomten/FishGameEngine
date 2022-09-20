#pragma once
#include <filesystem>

namespace ImGuiUtil
{
	std::filesystem::path GetSaveFilePath(const char* aFilter, const char* fileEnding);
	std::filesystem::path GetOpenFilePath(const char* aFilter);
	
	/// <summary>
	/// UNSAFE FUCTION, ONLY USE IF YOU KNOW WHAT YOU ARE DOING
	/// </summary>
	/// <param name="aActive"></param>
	void SetImGuiPayloadActive(bool aActive);
};