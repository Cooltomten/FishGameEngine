#pragma once
#include <cstdint>
#include <CommonUtilities/Math/Vector.hpp>
namespace CU = CommonUtilities;

namespace FGE
{
	class FrameBuffer
	{
	public:
		FrameBuffer(const CU::Vector2f& aSize);
		~FrameBuffer();

		void Resize(int aWidth, int aHeight);
		void ClearAttachment(uint32_t aIndex, const CU::Vector4f& aColor);
		void ClearDepth(float aDepth);

		void Bind() const;
		void Unbind() const;

		void BindColorView();
		void BindDepthVier();
		
		
		
	};
}
