#include "EditorSettings.h"
#include <nlohmann/json.hpp>
#include <fstream>
void EditorSettings::Save()
{
	nlohmann::json json;
	json["LastLoadedScene"] = myLastLoadedScene;
	json["LastBackgroundPath"] = myLastLoadedBackground;
	json["LastLoadedFirstBGP"] = myLastLoadedFirstBGP;
	json["LastLoadedSecondBGP"] = myLastLoadedSecondBGP;
	json["LastBlendValue"] = myLastBlendValue;
	json["LastBlendActive"] = myBlendActive;

	if (!std::filesystem::exists("User"))
	{
		std::filesystem::create_directory("User");
	}
	std::ofstream file("User/EditorSettings.json");
	file << json;
	file.close();
}

void EditorSettings::Load()
{
	myLastLoadedScene = "";
	myLastLoadedBackground = "";
	if (std::filesystem::exists("User/EditorSettings.json"))
	{
		std::ifstream file("User/EditorSettings.json");
		nlohmann::json json;
		file >> json;

		if (json.find("LastLoadedScene") != json.end())
		{
			myLastLoadedScene = json["LastLoadedScene"];
		}
		if (json.find("LastBackgroundPath") != json.end())
		{
			myLastLoadedBackground = json["LastBackgroundPath"];
		}
		if (json.find("LastLoadedFirstBGP") != json.end())
		{
			myLastLoadedFirstBGP = json["LastLoadedFirstBGP"];
		}
		if (json.find("LastLoadedSecondBGP") != json.end())
		{
			myLastLoadedSecondBGP = json["LastLoadedSecondBGP"];
		}
		if (json.find("LastBlendValue") != json.end())
		{
			myLastBlendValue = json["LastBlendValue"];
		}
		if (json.find("LastBlendActive") != json.end())
		{
			myBlendActive = json["LastBlendActive"];
		}
	}
	
}

CommonUtilities::Vector4f EditorSettings::LoadBackgroundFile(std::filesystem::path aPath)
{
	std::ifstream file(aPath);
	nlohmann::json json;
	file >> json;
	auto color = json["Color"];
	return { color[0], color[1], color[2], color[3] };
}
