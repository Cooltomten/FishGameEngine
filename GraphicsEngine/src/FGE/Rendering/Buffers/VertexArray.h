#pragma once

#include <memory>
struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;
namespace FGE
{

	class VertexBuffer;
	class IndexBuffer;

	class VertexArray
	{

	public:
		VertexArray();
		~VertexArray();

		void Bind() const;

		void SetVertexBuffer(std::shared_ptr<VertexBuffer> aVertexBuffer);
		void SetIndexBuffer(std::shared_ptr<IndexBuffer> aIndexBuffer);
		void SetInputLayout(ID3D11InputLayout* aInputLayout);

		inline const VertexBuffer& GetVertexBuffer() const { return *myVertexBuffer; }
		inline const IndexBuffer& GetIndexBuffer() const { return *myIndexBuffer; }
		inline  ID3D11InputLayout* GetInputLayout() { return myInputLayout; }


	private:
		std::shared_ptr<VertexBuffer> myVertexBuffer;
		std::shared_ptr<IndexBuffer> myIndexBuffer;

		ID3D11InputLayout* myInputLayout;
	};
}