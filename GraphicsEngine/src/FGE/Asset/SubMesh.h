#pragma once
#include <memory>
#include <vector>
#include "FGE/Rendering/Buffers/VertexArray.h"
#include "FGE/Rendering/Buffers/Vertex.h"
namespace FGE
{
	class SubMesh
	{
		public:
		SubMesh(const std::vector<Vertex>& aVertices, const std::vector<unsigned int>& aIndices);
		~SubMesh() = default;

		inline const std::shared_ptr<VertexArray> GetVertexArray() const { return myVertexArray; }
		
	private:
		std::shared_ptr<VertexArray> myVertexArray;
	};
}