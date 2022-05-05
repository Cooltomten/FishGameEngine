#include "GraphicsEngine.pch.h"
#include "Model.h"

namespace FGE
{

	void Mesh::Init(const MeshData& someMeshData, std::string aPath)
	{
		myMeshData = someMeshData;
		myName = aPath;
	}

	void Mesh::Render()
	{
		
	}


}