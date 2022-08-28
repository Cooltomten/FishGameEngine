#include "GraphicsEngine.pch.h"
#include "Mesh.h"

#include "FGE/Rendering/Renderer.h"

#include "FGE/Asset/SubMesh.h"
#include "FGE/Asset/Material.h"

namespace FGE
{

	void Mesh::Init(std::vector<std::shared_ptr<SubMesh>> aSubMesh)
	{
		mySubMeshes = aSubMesh;
		
		for (int i = 0; i < mySubMeshes.size(); i++)
		{
			myMaterials[mySubMeshes[i]->GetMaterialIndex()] = Material::Default;
		}
	}

	void Mesh::Render(const CU::Matrix4x4<float>& aTransform)
	{
		for (int i = 0; i < mySubMeshes.size(); i++)
		{
			
			Renderer::SubmitModel(mySubMeshes[i]->GetVertexArray(), aTransform, myMaterials[mySubMeshes[i]->GetMaterialIndex()]);
		}
	}
	void Mesh::SetMaterial(std::shared_ptr<Material> aMaterial, uint32_t aIndex)
	{
		myMaterials[aIndex] = aMaterial;
	}
	std::shared_ptr<Material> Mesh::GetMaterial(uint32_t aIndex)
	{
		return myMaterials[aIndex];
	}
}