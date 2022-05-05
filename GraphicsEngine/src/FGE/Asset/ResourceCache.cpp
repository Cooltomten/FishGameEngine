#include "GraphicsEngine.pch.h"
#include "ResourceCache.h"
#include "FGE/Asset/Vertex.h"
#include "FGE/Core/Application.h"
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
		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.ByteWidth = static_cast<UINT>(static_cast<UINT>(sizeof(Vertex)) * someVertices.size());
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData;
		vertexSubResourceData.pSysMem = someVertices.data();

		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		ID3D11Buffer* vertexBuffer = dx11.CreateBuffer(&vertexBufferDesc, &vertexSubResourceData);
		//end createvertex buffer

		//Create index buffer
		D3D11_BUFFER_DESC indexBufferDesc;
		indexBufferDesc.ByteWidth = static_cast<UINT>(static_cast<UINT>(sizeof(unsigned int)) * someIndices.size());
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;
		
		D3D11_SUBRESOURCE_DATA indexSubResourceData;
		indexSubResourceData.pSysMem = someIndices.data();
		
		ID3D11Buffer* indexBuffer = dx11.CreateBuffer(&indexBufferDesc, &indexSubResourceData);
		//end create index buffer

		//Create shaders
		std::ifstream vsFile;
		vsFile.open("DefaultVS.cso", std::ios::binary);
		std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
		vsFile.close();
		ID3D11VertexShader* vertexShader = dx11.CreateVertexShader(vsData.data(), vsData.size());

		std::ifstream psFile;
		psFile.open("DefaultPS.cso", std::ios::binary);
		std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
		psFile.close();
		ID3D11PixelShader* pixelShader = dx11.CreatePixelShader(psData.data(), psData.size());
		//end shader
		
		//Create input layout 
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		
		ID3D11InputLayout* inputLayout = dx11.CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vsData.data(), vsData.size());
		//End layout
		
		Mesh::MeshData meshData;
		meshData.NumberOfVertices = static_cast<UINT>(someVertices.size());
		meshData.NumberOfIndices = static_cast<UINT>(someIndices.size());
		meshData.Stride = sizeof(Vertex);
		meshData.Offset = 0;
		meshData.VertexBuffer = vertexBuffer;
		meshData.IndexBuffer = indexBuffer;
		meshData.VertexShader = vertexShader;
		meshData.PixelShader = pixelShader;
		meshData.PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		meshData.InputLayout = inputLayout;

		std::shared_ptr<Mesh> mdl = std::make_shared<Mesh>();
		mdl->Init(meshData, aName);
		
		return mdl;
	}

}