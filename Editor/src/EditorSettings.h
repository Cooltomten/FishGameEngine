#pragma once
#include <string>
#include <CommonUtilities/Math/Vector.hpp>
#include <filesystem>

class EditorSettings
{
public:
	void Save();
	void Load();

	CommonUtilities::Vector4f LoadBackgroundFile(std::filesystem::path aPath); // should be in a background class but one doesnt exist
	
	std::string myLastLoadedScene = "";
	std::string myLastLoadedBackground = "";

	std::string myLastLoadedFirstBGP = "";
	std::string myLastLoadedSecondBGP = "";
	float myLastBlendValue = -1;

	bool myBlendActive = true;
};