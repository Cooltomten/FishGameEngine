#pragma once
#include <memory>
struct ID3D11InputLayout;
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
			
			void SetVertexBuffer();
			void SetIndexBuffer();
			
			inline const VertexBuffer& GetVertexBuffer() const { return *myVertexBuffer; }
			inline const IndexBuffer& GetIndexBuffer() const { return *myIndexBuffer; }
		
			private:
				std::shared_ptr<VertexBuffer> myVertexBuffer;
				std::shared_ptr<IndexBuffer> myIndexBuffer;
				
				ID3D11InputLayout* myInputLayout;
	};
}