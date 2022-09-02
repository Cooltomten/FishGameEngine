#pragma once
#include <ComponentSystem/Component.h>
#include <memory>
#include <vector>

namespace FGE
{
	class Mesh;
	class Material;
}
namespace Engine
{
	class MeshRenderer : public Comp::Component
	{
	public:
		MeshRenderer();
		~MeshRenderer() = default;

		void Initialize() override;
		void OnEvent(FGE::Event& aEvent) override;
		
		static std::string GetFactoryName() { return "MeshRenderer"; }
		static std::shared_ptr<Comp::Component> Create() { return std::make_shared<MeshRenderer>(); }

	private:
		std::shared_ptr<FGE::Mesh> myMesh;
		std::vector<std::shared_ptr<FGE::Material>> myMaterials;

		std::string myMeshPath;
		std::string myLastMeshPath;

		std::string myMaterialPath;
		std::string myLastMaterialPath;



	};
}