#include "GraphicsEngine.pch.h"
#include "Model.h"

namespace FGE
{

	void Model::Init(const MeshData& someMeshData, std::string aPath)
	{
		myMeshData = someMeshData;
		myName = aPath;
	}

	Transform& Model::GetTransform()
	{
		return myTransform;
	}


}