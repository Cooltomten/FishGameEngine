#pragma once
#include <ComponentSystem/Component.h>
#include <memory>
#include <vector>

namespace FGE
{
	class ParticleEmitter;
	class EmitterSettingsData;
}
namespace Engine
{
	class ParticleSystem : public Comp::Component
	{
	public:
		ParticleSystem();
		~ParticleSystem() = default;

		void Initialize() override;
		void OnEvent(FGE::Event& aEvent) override;

		static std::string GetFactoryName() { return "ParticleSystem"; }
		static std::shared_ptr<Comp::Component> Create() { return std::make_shared<ParticleSystem>(); }

	private:
		std::shared_ptr<FGE::ParticleEmitter> myEmitter;

		std::string myEmitterPath;
		std::string myPrevEmitterPath;

		std::shared_ptr<FGE::EmitterSettingsData> myEmitterData;



	};
}