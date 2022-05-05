#include "GraphicsEngine.pch.h"
#include "VertexArray.h"

#include "FGE/Rendering/Buffers/VertexBuffer.h"
#include "FGE/Rendering/Buffers/IndexBuffer.h"

#include "FGE/Core/Application.h"
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

		Application::Get().GetWindow()->GetDX11().GetDeviceContext()->IASetInputLayout(myInputLayout);
	}

	void VertexArray::SetVertexBuffer()
	{
	}

	void VertexArray::SetIndexBuffer()
	{
	}

}