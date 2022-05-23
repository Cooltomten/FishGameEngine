#include "GraphicsEngine.pch.h"
#include "AnimatedMesh.h"

#include "FGE/Asset/SubMesh.h"

#include "FGE/Rendering/Renderer.h"

namespace FGE
{

	void FGE::AnimatedMesh::Init(std::vector<std::shared_ptr<SubMesh>> aSubMesh, Skeleton& aSkeleton)
	{
		mySubMeshes = aSubMesh;
		//memcpy skeleton into mySkeleton
		memcpy_s(&mySkeleton, sizeof(Skeleton), &aSkeleton, sizeof(Skeleton));
	}

	void FGE::AnimatedMesh::Render(const CU::Matrix4x4<float>& aTransform)
	{
		for (int i = 0; i < mySubMeshes.size(); i++)
		{
			Renderer::Submit(mySubMeshes[i]->GetVertexArray(), aTransform, myMaterials[mySubMeshes[i]->GetMaterialIndex()]);
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