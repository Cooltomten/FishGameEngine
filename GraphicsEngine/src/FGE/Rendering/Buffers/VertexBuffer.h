#pragma once
struct ID3D11Buffer;

namespace FGE
{
	enum class BufferUsage
	{
		Default,
		Immutable, 
		Dynamic,
		Staging
	};

	struct BufferDescriptor
	{
		BufferDescriptor(UINT aSize, UINT aStride, void* aData, BufferUsage aUsage) : Size(aSize), Stride(aStride), Data(aData), Usage(aUsage) {}

		UINT Size;
		UINT Stride;
		void* Data;
		BufferUsage Usage;
	};

	class Buffer
	{
	public:
		Buffer()
			: myBuffer(nullptr)
		{
			
		}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	protected:
		ID3D11Buffer* myBuffer;
		
	};

	class VertexBuffer : public Buffer
	{
	public:
		VertexBuffer(UINT aByteWidth, UINT aUsage, UINT aBindFlags, const void* aData);

		// Inherited via Buffer
		virtual void Bind() override;
		virtual void Unbind() override;
	};

	class IndexBuffer : public Buffer
	{
		// Inherited via Buffer
		virtual void Bind() override;
		virtual void Unbind() override;
	};
}