#pragma once
namespace FGE
{
	class ParticleEmitter;
	class EmitterSettingsData;
	class EmitterSettingsImporter
	{
	public:
		EmitterSettingsImporter() = default;
		~EmitterSettingsImporter() = default;
		std::shared_ptr<EmitterSettingsData> ImportEmitterSettings(const std::filesystem::path& aPath);

	};
}