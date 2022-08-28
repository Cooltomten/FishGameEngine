#include "GraphicsEngine.pch.h"
#include "VertexBuffer.h"
#include "FGE/Core/Application.h"

namespace FGE
{
	VertexBuffer::VertexBuffer(UINT aByteWidth, UINT aUsage, UINT aStride, const void* aData, UINT someCPUAccessFlags)
	{
		myByteWidth = aByteWidth;
		myUsage = aUsage;
		myStride = aStride;

		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.ByteWidth = aByteWidth;
		vertexBufferDesc.Usage = static_cast<D3D11_USAGE>(aUsage);
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = someCPUAccessFlags;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData;
		ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
		
		vertexSubResourceData.pSysMem = aData;

		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		ID3D11Buffer* vertexBuffer = dx11.CreateBuffer(&vertexBufferDesc, &vertexSubResourceData);
		myBuffer = vertexBuffer;
	}
	void VertexBuffer::Bind()
	{
		auto& context = Application::Get().GetWindow()->GetDX11().GetDeviceContext();
		const UINT stride = myStride;
		const UINT offset = 0;
		
		context->IASetVertexBuffers(0, 1, myBuffer.GetAddressOf(), &stride, &offset);
	}
	void VertexBuffer::Unbind()
	{
	}
	
	IndexBuffer::IndexBuffer(UINT aByteWidth, UINT aUsage, const void* aData)
	{
		myCount = aByteWidth/sizeof(UINT);
		
		D3D11_BUFFER_DESC indexBufferDesc;
		indexBufferDesc.ByteWidth = aByteWidth;
		indexBufferDesc.Usage = static_cast<D3D11_USAGE>(aUsage);
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexSubResourceData;
		ZeroMemory(&indexSubResourceData, sizeof(indexSubResourceData));
		
		indexSubResourceData.pSysMem = aData;
		
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		ID3D11Buffer* indexBuffer = dx11.CreateBuffer(&indexBufferDesc, &indexSubResourceData);
		myBuffer = indexBuffer;
	}

	void IndexBuffer::Bind()
	{
		auto& context = Application::Get().GetWindow()->GetDX11().GetDeviceContext();
		context->IASetIndexBuffer(myBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	}
	void IndexBuffer::Unbind()
	{
	}
}
