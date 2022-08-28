#include "GraphicsEngine.pch.h"
#include "EmitterSettingsImporter.h"
#include "FGE/Asset/EmitterSettingsData.h"
#include <nlohmann/json.hpp>
namespace FGE
{
	std::shared_ptr<EmitterSettingsData> FGE::EmitterSettingsImporter::ImportEmitterSettings(const std::filesystem::path& aPath)
	{
		std::ifstream loadFile;
		loadFile.open(aPath);

		nlohmann::json doc;
		loadFile >> doc;

		EmitterSettingsData data;
		data.LifeTime = doc["LifeTime"].get<float>();
		data.ParticlesPerSecond = doc["ParticlesPerSecond"].get<float>();
		data.StartColor = CU::Vector4f(doc["StartColor"]["R"], doc["StartColor"]["G"], doc["StartColor"]["B"], doc["StartColor"]["A"]);
		data.StartScale = doc["StartScale"].get<float>();
		data.StartVelocity = CU::Vector3f(doc["StartVelocity"]["X"], doc["StartVelocity"]["Y"], doc["StartVelocity"]["Z"]);

		return std::make_shared<EmitterSettingsData>(data);
	}
}
