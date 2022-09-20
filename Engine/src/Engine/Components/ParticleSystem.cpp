#include "ParticleSystem.h"
#include "Engine/Event/ApplicationEvents.h"
#include "Engine/Event/EntityEvents.h"

#include <ComponentSystem/ComponentRegistry.hpp>
#include <ComponentSystem/Entity.h>

#include <FGE/Particles/ParticleEmitter.h>

#include <FGE/Rendering/Renderer.h>

#include <FGE/Asset/ResourceCache.h>
#include <FGE/Asset/EmitterSettingsData.h>
namespace Engine
{
	REGISTER_COMPONENT(ParticleSystem);

	Engine::ParticleSystem::ParticleSystem()
		: Comp::Component("ParticleSystem")
	{
		myEmitter = std::make_shared<FGE::ParticleEmitter>();
		InitializeParameters({
	{Comp::ParameterType::String, "EmitterData Path", &myEmitterPath},
			});
	}

	void Engine::ParticleSystem::Initialize()
	{
	}

	void Engine::ParticleSystem::OnEvent(FGE::Event& aEvent)
	{
		FGE::EventDispatcher dispatcher(aEvent);

		dispatcher.Dispatch<FGE::AppUpdateEvent>([this](FGE::AppUpdateEvent& aEvent)
			{
				if (myEmitterData)
				{
					myEmitter->Update(aEvent.GetTimeStep());
				}
				return false;
			});

		dispatcher.Dispatch<FGE::AppRenderEvent>([this](FGE::AppRenderEvent& aEvent)
			{
				if (myEmitterData)
				{
					myEmitter->Render(myEntity->GetTransform().GetMatrix());
				}
				return false;
			});

		dispatcher.Dispatch<EntityPropertyUpdatedEvent>([this](EntityPropertyUpdatedEvent& aEvent)
			{
				if (myEmitterPath != myPrevEmitterPath)
				{
					myPrevEmitterPath = myEmitterPath;
					if (std::filesystem::exists(myEmitterPath) && std::filesystem::is_regular_file(myEmitterPath))
					{
						auto emitterSettings = FGE::ResourceCache::GetAsset<FGE::EmitterSettingsData>(myEmitterPath);
						if (myEmitterData != emitterSettings)
						{
							myEmitterData = emitterSettings;
							myEmitter->Init(myEmitterData);
						}
					}
				}


				return false;
			});

	}

}