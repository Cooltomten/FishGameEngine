#include "GraphicsEngine.pch.h"
#include "VertexArray.h"

#include "FGE/Rendering/Buffers/VertexBuffer.h"

#include "FGE/Core/DX11.h"

#include "FGE/Core/Window.h"

namespace FGE
{
	VertexArray::VertexArray()
		: myVertexBuffer(nullptr), myIndexBuffer(nullptr), myInputLayout(nullptr)
	{
		
	}

	VertexArray::~VertexArray()
	{
		if (myInputLayout)
		{
			myInputLayout->Release();
			myInputLayout = nullptr;
		}
	}

	void VertexArray::Bind() const
	{
		myVertexBuffer->Bind();
		myIndexBuffer->Bind();

		Window::Get().GetDX11().GetDeviceContext()->IASetInputLayout(myInputLayout);
	}

	void VertexArray::SetVertexBuffer(std::shared_ptr<VertexBuffer> aVertexBuffer)
	{
		myVertexBuffer = aVertexBuffer;
	}

	void VertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> aIndexBuffer)
	{
		myIndexBuffer = aIndexBuffer;
	}

	void VertexArray::SetInputLayout(ID3D11InputLayout* aInputLayout)
	{
		myInputLayout = aInputLayout;
	}

}