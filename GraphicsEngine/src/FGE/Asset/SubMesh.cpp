#include "GraphicsEngine.pch.h"
#include "SubMesh.h"
#include "FGE/Core/Application.h"
#include "FGE/Rendering/Renderer.h"
#include "FGE/Rendering/Buffers/VertexBuffer.h"

namespace FGE
{
	SubMesh::SubMesh(const std::vector<Vertex>& aVertices, const std::vector<unsigned int>& aIndices)
	{
		//Create vertex buffer
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(static_cast<UINT>(static_cast<UINT>(sizeof(Vertex)) * aVertices.size()), D3D11_USAGE_IMMUTABLE, sizeof(Vertex), aVertices.data());

		//Create index buffer
		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(static_cast<UINT>(static_cast<UINT>(sizeof(unsigned int)) * aIndices.size()), D3D11_USAGE_IMMUTABLE, aIndices.data());

		//Create input layout 
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 3, DXGI_FORMAT_R32G32_FLOAT, 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEIDS", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		ID3D11InputLayout* inputLayout = dx11.CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), Renderer::GetVsData().data(), Renderer::GetVsData().size());
		//End layout
		
		std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>();
		vertexArray->SetVertexBuffer(vertexBuffer);
		vertexArray->SetIndexBuffer(indexBuffer);
		vertexArray->SetInputLayout(inputLayout);
		myVertexArray = vertexArray;

		
	}
}