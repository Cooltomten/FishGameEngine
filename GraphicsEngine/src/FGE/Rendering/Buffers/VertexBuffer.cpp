#include "GraphicsEngine.pch.h"
#include "VertexBuffer.h"

namespace FGE
{
	VertexBuffer::VertexBuffer(UINT aByteWidth, UINT aUsage, UINT aBindFlags, const void* aData)
	{
		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.ByteWidth = aByteWidth;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData;
		vertexSubResourceData.pSysMem = someVertices.data();

		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		ID3D11Buffer* vertexBuffer = dx11.CreateBuffer(&vertexBufferDesc, &vertexSubResourceData);
	}
	void VertexBuffer::Bind()
	{
	}
	void VertexBuffer::Unbind()
	{
	}
	void IndexBuffer::Bind()
	{
	}
	void IndexBuffer::Unbind()
	{
	}
}
