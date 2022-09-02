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
	}

	void FGE::AnimatedMesh::Render(const CU::Matrix4x4<float>& aTransform, const std::vector<std::shared_ptr<Material>>& someMaterials, std::shared_ptr<Animation> aAnimation,
		std::shared_ptr<Animation> aAnimationToBlendWith, float aBlendAlpha, float aAnimationTime)
	{
		std::vector<CU::Matrix4x4<float>> matrices;

		matrices = aAnimation->Sample(aAnimationTime, mySkeleton, aAnimationToBlendWith, aBlendAlpha);

		for (int i = 0; i < mySubMeshes.size(); i++)
		{
			Renderer::SubmitModel(mySubMeshes[i]->GetVertexArray(), aTransform, someMaterials[0], matrices);
		}
	}

}