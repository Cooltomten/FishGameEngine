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
	}

	void Mesh::Render(const CU::Matrix4x4<float>& aTransform, const std::vector<std::shared_ptr<Material>>& someMaterials)
	{
		for (int i = 0; i < mySubMeshes.size(); i++)
		{		
			Renderer::SubmitModel(mySubMeshes[i]->GetVertexArray(), aTransform, someMaterials[0]);
		}
	}

}