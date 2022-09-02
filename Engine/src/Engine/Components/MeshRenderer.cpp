#include "MeshRenderer.h"
#include "Engine/Event/ApplicationEvents.h"
#include "Engine/Event/EntityEvents.h"

#include <ComponentSystem/ComponentRegistry.hpp>
#include <ComponentSystem/Entity.h>

#include <FGE/Rendering/Renderer.h>

#include <FGE/Asset/ResourceCache.h>
#include <FGE/Asset/Material.h>

namespace Engine
{
	REGISTER_COMPONENT(MeshRenderer);

	Engine::MeshRenderer::MeshRenderer()
		: Comp::Component("MeshRenderer")
	{
		myMeshPath = "Assets/Meshes/SM_Particle_Chest.fbx";
		myLastMeshPath = myMeshPath;
		myMaterialPath = "Assets/Materials/DefaultMaterial.material";
		myLastMaterialPath = myMaterialPath;
		InitializeParameters({
			{Comp::ParameterType::String, "Mesh Path", &myMeshPath},
			{Comp::ParameterType::String, "Material Path", &myMaterialPath}
			});
	}

	void Engine::MeshRenderer::Initialize()
	{
		myMesh = FGE::ResourceCache::GetAsset<FGE::Mesh>(myMeshPath);

		myMaterials.push_back( FGE::ResourceCache::GetAsset<FGE::Material>(myMaterialPath));
	}

	void Engine::MeshRenderer::OnEvent(FGE::Event& aEvent)
	{
		FGE::EventDispatcher dispatcher(aEvent);

		dispatcher.Dispatch<FGE::AppRenderEvent>([this](FGE::AppRenderEvent& aEvent)
			{
				myMesh->Render(myEntity->GetTransform().GetMatrix(), myMaterials);
				return false;
			});

		dispatcher.Dispatch<EntityPropertyUpdatedEvent>(
			[this](EntityPropertyUpdatedEvent& aEvent)
			{
				if (myMeshPath != myLastMeshPath)
				{
					myLastMeshPath = myMeshPath;
					if (std::filesystem::exists(myMeshPath) && std::filesystem::is_regular_file(myMeshPath))
					{
						myMesh = FGE::ResourceCache::GetAsset<FGE::Mesh>(myMeshPath);
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

				return false;
			}
		);


	}


}