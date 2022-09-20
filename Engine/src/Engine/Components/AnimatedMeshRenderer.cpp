#include "AnimatedMeshRenderer.h"
#include "Engine/Event/ApplicationEvents.h"
#include "Engine/Event/EntityEvents.h"

#include <ComponentSystem/ComponentRegistry.hpp>
#include <ComponentSystem/Entity.h>

#include <FGE/Rendering/Renderer.h>

#include <FGE/Asset/ResourceCache.h>
#include <FGE/Asset/Material.h>
#include <FGE/Asset/AnimatedMesh.h>
#include <FGE/Asset/Animation.h>

#include <CommonUtilities/UtilityFunctions.hpp>

namespace Engine
{
	REGISTER_COMPONENT(AnimatedMeshRenderer);

	Engine::AnimatedMeshRenderer::AnimatedMeshRenderer()
		: Comp::Component("AnimatedMeshRenderer")
	{
		myAnimationSpeed = 1;
		myBlendFactor = 0;
		myTimer = 0;
		myMaterials.push_back(FGE::Material::Default);
		InitializeParameters(
			{
				{Comp::ParameterType::String, "Mesh Path", &myMeshPath},
				{Comp::ParameterType::String, "Material Path", &myMaterialPath},
				{Comp::ParameterType::String, "First Animation Path", &myFirstAnimationPath},
				{Comp::ParameterType::String, "Second Animation Path", &mySecondAnimationPath},
				{Comp::ParameterType::Float, "Animation Speed", &myAnimationSpeed},
				{Comp::ParameterType::Float, "Blend Factor", &myBlendFactor},

			});

	}

	void Engine::AnimatedMeshRenderer::Initialize()
	{
	}

	void Engine::AnimatedMeshRenderer::OnEvent(FGE::Event& aEvent)
	{
		FGE::EventDispatcher dispatcher(aEvent);

		dispatcher.Dispatch<FGE::AppUpdateEvent>([this](FGE::AppUpdateEvent& aEvent)
			{
				if (myFirstAnimation && myMesh)
				{
					myTimer += aEvent.GetTimeStep() * myAnimationSpeed;

					auto& secondAnim = mySecondAnimation;
					if (!secondAnim)
					{
						secondAnim = myFirstAnimation;
					}
					float duration = CU::Lerp(myFirstAnimation->GetDuration(), secondAnim->GetDuration(), myBlendFactor);
					if (myTimer >= duration)
					{
						myTimer = 0;
					}
					if (myTimer < 0) // in the case animation speed is negative
					{
						myTimer = duration;
					}
					if (myTimer)
					{

					}
				}
				return false;
			});

		dispatcher.Dispatch<FGE::AppRenderEvent>([this](FGE::AppRenderEvent& aEvent)
			{
				if (myMesh && myMaterials.size() > 0)
				{
					auto& secondAnim = mySecondAnimation;
					if (!secondAnim)
					{
						secondAnim = myFirstAnimation;
					}
					myMesh->Render(myEntity->GetTransform().GetMatrix(), myMaterials, myFirstAnimation, secondAnim, myBlendFactor, myTimer);
				}
				return false;
			});

		dispatcher.Dispatch<EntityPropertyUpdatedEvent>([this](EntityPropertyUpdatedEvent& aEvent)
			{
				if (myMeshPath != myLastMeshPath)
				{
					myLastMeshPath = myMeshPath;
					if (std::filesystem::exists(myMeshPath) && std::filesystem::is_regular_file(myMeshPath))
					{
						myMesh = FGE::ResourceCache::GetAsset<FGE::AnimatedMesh>(myMeshPath);
					}
				}

				if (myMaterialPath != myLastMaterialPath)
				{
					myLastMaterialPath = myMaterialPath;
					if (std::filesystem::exists(myMaterialPath) && std::filesystem::is_regular_file(myMaterialPath))
					{
						myMaterials[0] = FGE::ResourceCache::GetAsset<FGE::Material>(myMaterialPath);
					}
				}

				if (myFirstAnimationPath != myPrevFirstAnimationPath)
				{
					myPrevFirstAnimationPath = myFirstAnimationPath;
					if (std::filesystem::exists(myFirstAnimationPath) && std::filesystem::is_regular_file(myFirstAnimationPath))
					{
						myFirstAnimation = FGE::ResourceCache::GetAsset<FGE::Animation>(myFirstAnimationPath);
					}
				}

				if (mySecondAnimationPath != myPrevSecondAnimationPath)
				{
					myPrevSecondAnimationPath = mySecondAnimationPath;
					if (std::filesystem::exists(mySecondAnimationPath) && std::filesystem::is_regular_file(mySecondAnimationPath))
					{
						mySecondAnimation = FGE::ResourceCache::GetAsset<FGE::Animation>(mySecondAnimationPath);
					}
				}

				return false;
			});
	}

}