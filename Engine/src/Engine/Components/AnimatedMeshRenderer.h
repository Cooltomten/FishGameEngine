#pragma once
#include <ComponentSystem/Component.h>
#include <memory>
#include <vector>

namespace FGE
{
	class AnimatedMesh;
	class Animation;
	class Material;
}
namespace Engine
{
	class AnimatedMeshRenderer : public Comp::Component
	{
	public:
		AnimatedMeshRenderer();
		~AnimatedMeshRenderer() = default;

		void Initialize() override;
		void OnEvent(FGE::Event& aEvent) override;

		static std::string GetFactoryName() { return "AnimatedMeshRenderer"; }
		static std::shared_ptr<Comp::Component> Create() { return std::make_shared<AnimatedMeshRenderer>(); }

	private:
		std::shared_ptr<FGE::AnimatedMesh> myMesh;
		std::vector<std::shared_ptr<FGE::Material>> myMaterials;
		std::shared_ptr<FGE::Animation> myFirstAnimation;
		std::shared_ptr<FGE::Animation> mySecondAnimation;
		

		float myTimer;
		float myBlendFactor;
		float myAnimationSpeed;

		std::string myMeshPath;
		std::string myLastMeshPath;

		std::string myMaterialPath;
		std::string myLastMaterialPath;

		std::string myFirstAnimationPath;
		std::string myPrevFirstAnimationPath;
		
		std::string mySecondAnimationPath;
		std::string myPrevSecondAnimationPath;
		



	};
}