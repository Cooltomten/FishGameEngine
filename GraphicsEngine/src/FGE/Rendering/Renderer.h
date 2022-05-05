#pragma once
#include "FGE/Rendering/Buffers/"
#include <memory>
#include <CommonUtilities/Math/Matrix4x4.hpp>

namespace CU = CommonUtilities;
namespace FGE
{

	struct SubmitCommand
	{
		std::shared<VertexArray> Data;
		CU::Matrix4x4<float> Transform;
	};
	class Renderer
	{
	public:
		void Submit();
	};
}