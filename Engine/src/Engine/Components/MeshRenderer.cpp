#include "MeshRenderer.h"
#include "Engine/Event/ApplicationEvents.h"

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
	}

	void Engine::MeshRenderer::Initialize()
	{

		myMesh = FGE::ResourceCache::GetAsset<FGE::Mesh>("Assets/Meshes/SM_Particle_Chest.fbx");
		
		myMaterial = std::make_shared<FGE::Material>();
		myMaterial->SetTexture(FGE::MaterialTextureChannel::Albedo, FGE::ResourceCache::GetAsset<FGE::Texture>("Assets/Textures/T_Particle_Chest_C.dds"));
		myMaterial->SetTexture(FGE::MaterialTextureChannel::Normal, FGE::ResourceCache::GetAsset<FGE::Texture>("Assets/Textures/T_Particle_Chest_N.dds"));
		myMaterial->SetTexture(FGE::MaterialTextureChannel::Material, FGE::ResourceCache::GetAsset<FGE::Texture>("Assets/Textures/T_Particle_Chest_M.dds"));
		myMesh->SetMaterial(myMaterial, 0);
	}

	void Engine::MeshRenderer::OnEvent(FGE::Event& aEvent)
	{
		FGE::EventDispatcher dispatcher(aEvent);

		dispatcher.Dispatch<FGE::AppRenderEvent>([this](FGE::AppRenderEvent& aEvent) 
			{
				myMesh->Render(myEntity->GetTransform().GetMatrix());
				return false;
			});
	}


}