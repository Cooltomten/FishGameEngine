#include "MeshRendererComponent.h"

#include "ComponentSystem/ComponentRegistry.hpp"

namespace FGE
{

	REGISTER_COMPONENT(MeshRendererComponent);

	FGE::MeshRendererComponent::MeshRendererComponent()
		: Comp::Component("MeshRendererComponent")
	{
	}

	void FGE::MeshRendererComponent::Initialize()
	{
	}

	void FGE::MeshRendererComponent::OnEvent(const FGE::Event& aEvent)
	{
	}
}
