#include "GraphicsEngine.pch.h"
#include "Model.h"
#include "FGE/Rendering/Renderer.h"


namespace FGE
{

	void Mesh::Init(std::shared_ptr<VertexArray> aVertexArray, std::string aPath)
	{
		myName = aPath;
		myVertexArray = aVertexArray;

	}

	void Mesh::Render(const CU::Matrix4x4<float>& aTransform)
	{
		Renderer::Submit(myVertexArray, aTransform);
	}



}