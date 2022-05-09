#pragma once
#include <wrl.h>

using namespace Microsoft::WRL;

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
		ComPtr<ID3D11Buffer> myBuffer;
		UINT myUsage;

	};

	class VertexBuffer : public Buffer
	{
	public:
		VertexBuffer(UINT aByteWidth, UINT aUsage, UINT aStride, const void* aData);

		// Inherited via Buffer
		virtual void Bind() override;
		virtual void Unbind() override;

	private:
		UINT myByteWidth;
		UINT myStride;
	};

	class IndexBuffer : public Buffer
	{
	public:
		IndexBuffer(UINT aByteWidth, UINT aUsage, const void* aData);

		// Inherited via Buffer
		virtual void Bind() override;
		virtual void Unbind() override;

		inline int GetCount() const { return myCount; }

	private:
		int myCount;
	};
}