#include "GraphicsEngine.pch.h"
#include "ResourceCache.h"
#include "FGE/Asset/Vertex.h"
#include "FGE/Core/Application.h"
#include "FGE/Rendering/Buffers/VertexBuffer.h"
#include "FGE/Rendering/Buffers/VertexArray.h"
#include "FGE/Rendering/Renderer.h"
#include <fstream>

namespace FGE
{
	std::unordered_map<std::string, std::shared_ptr<Mesh>> ResourceCache::myAssets;

	void ResourceCache::Initialize()
	{
		SaveAsset("Cube", CreateUnitCube());
	}

	void ResourceCache::SaveAsset(const std::string& aName, std::shared_ptr<Mesh> aModel)
	{
		myAssets[aName] = aModel;
	}

	std::shared_ptr<Mesh> ResourceCache::GetAsset(const std::string& aName)
	{
		return myAssets[aName];
	}

	std::shared_ptr<Mesh> ResourceCache::CreateUnitCube()
	{
		const float s = 50.f;
		std::vector<Vertex> mdlVertices =
		{
			{
				s,s,s,
				1,0,0,1
			},
			{
				s,s,-s,
				0,1,0,1
			},
			{
				s,-s,s,
				0,0,1,1
			},
			{
				s, -s, -s,
				0,1,1,1
			},
			{
				-s,s,-s,
				1,1,0,1
			},
			{
				-s, s, s,
				1,0,1,1
			},
			{
				-s,-s,-s,
				0,0,0,1
			},
			{
				-s, -s, s,
				1,1,1,1
			}
		};
		std::vector<unsigned int> mdlIndices =
		{
			0,2,1,
			1,2,3,
			1,3,4,
			4,3,6,
			4,6,5,
			5,6,7,
			5,7,0,
			0,7,2,
			2,7,3,
			3,7,6,
			5,0,1,
			4,5,1
		};

		std::shared_ptr<Mesh> mdl = CreateModelFromVertices(mdlVertices, mdlIndices, "Cube");


		return mdl;
	}

	std::shared_ptr<Mesh> ResourceCache::CreateModelFromVertices(const std::vector<Vertex>& someVertices, const std::vector<unsigned int>& someIndices, std::string aName)
	{
		//Create vertex buffer
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(static_cast<UINT>(static_cast<UINT>(sizeof(Vertex)) * someVertices.size()), D3D11_USAGE_IMMUTABLE, sizeof(Vertex), someVertices.data());

		//Create index buffer
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(static_cast<UINT>(static_cast<UINT>(sizeof(unsigned int)) * someIndices.size()), D3D11_USAGE_IMMUTABLE, someIndices.data());
		
		//Create input layout 
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		
		ID3D11InputLayout* inputLayout = dx11.CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), Renderer::GetVsData().data(), Renderer::GetVsData().size());
		//End layout
		std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>();
		vertexArray->SetVertexBuffer(vertexBuffer);
		vertexArray->SetIndexBuffer(indexBuffer);
		vertexArray->SetInputLayout(inputLayout);
		

		std::shared_ptr<Mesh> mdl = std::make_shared<Mesh>();

		mdl->Init(vertexArray, aName);
		
		return mdl;
	}

}