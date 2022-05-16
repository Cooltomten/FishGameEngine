#include "GraphicsEngine.pch.h"
#include "Mesh.h"

#include "FGE/Rendering/Renderer.h"

#include "FGE/Asset/SubMesh.h"
#include "FGE/Asset/Material.h"

namespace FGE
{

	void Mesh::Init(std::string aPath, std::vector<std::shared_ptr<SubMesh>> aSubMesh)
	{
		myName = aPath;
		mySubMeshes = aSubMesh;
	}

	void Mesh::Render(const CU::Matrix4x4<float>& aTransform)
	{
		for (int i = 0; i < mySubMeshes.size(); i++)
		{
			Renderer::Submit(mySubMeshes[i]->GetVertexArray(), aTransform, std::make_shared<Material>());
		}
	}
}