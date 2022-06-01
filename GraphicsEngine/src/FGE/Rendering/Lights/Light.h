#pragma once
#include <CommonUtilities/Math/Vector3.hpp>

namespace CU = CommonUtilities;

namespace FGE
{
	class Light
	{
	public:
		struct LightBufferData
		{
			CU::Vector3f Color;
			float Intensity;
			alignas(16)CU::Vector3f Direction;
		};

	public:
		Light() = default;
		void Init(const CU::Vector3f& aColor, float aIntensity);

		virtual void SetAsResource();
		
	protected:
		LightBufferData myLightBufferData;
		
		
	};
}