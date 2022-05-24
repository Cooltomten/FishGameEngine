#pragma once

#include "ComponentSystem/Component.h"

#include <memory>

namespace FGE
{
	class Event;
	class MeshRendererComponent : public Comp::Component
	{
	public:
		MeshRendererComponent();
		~MeshRendererComponent() = default;

		void Initialize() override;
		void OnEvent(const FGE::Event& aEvent);

		static std::string GetFactoryName(){return "MeshRendererComponent";}
		static std::shared_ptr<Component>Create() { return std::make_shared<MeshRendererComponent>(); }
	};
}