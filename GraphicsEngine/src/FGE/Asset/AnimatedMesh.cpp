#include "GraphicsEngine.pch.h"
#include "AnimatedMesh.h"

#include "FGE/Asset/SubMesh.h"
#include "FGE/Asset/Material.h"

#include "FGE/Rendering/Renderer.h"


namespace FGE
{

	void FGE::AnimatedMesh::Init(std::vector<std::shared_ptr<SubMesh>> aSubMesh, Skeleton& aSkeleton)
	{
		mySubMeshes = aSubMesh;
		mySkeleton = aSkeleton;

		for (int i = 0; i < mySubMeshes.size(); i++)
		{
			myMaterials[mySubMeshes[i]->GetMaterialIndex()] = Material::Default;
		}
	}

	void FGE::AnimatedMesh::Render(const CU::Matrix4x4<float>& aTransform, std::shared_ptr<Animation> aAnimation,
		std::shared_ptr<Animation> aAnimationToBlendWith, float aBlendAlpha, float aAnimationTime)
	{
		std::vector<CU::Matrix4x4<float>> matrices;

		matrices = aAnimation->Sample(aAnimationTime, mySkeleton, aAnimationToBlendWith, aBlendAlpha);

		for (int i = 0; i < mySubMeshes.size(); i++)
		{
			Renderer::SubmitModel(mySubMeshes[i]->GetVertexArray(), aTransform, myMaterials[mySubMeshes[i]->GetMaterialIndex()], matrices);
		}
	}

	void FGE::AnimatedMesh::SetMaterial(std::shared_ptr<Material> aMaterial, uint32_t aIndex)
	{
		myMaterials[aIndex] = aMaterial;
	}

	std::shared_ptr<Material> FGE::AnimatedMesh::GetMaterial(uint32_t aIndex)
	{
		return myMaterials[aIndex];
	}

}