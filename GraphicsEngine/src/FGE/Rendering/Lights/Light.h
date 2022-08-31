#pragma once
#include <CommonUtilities/Math/Vector3.hpp>

//for comptr
#include <wrl.h>
using namespace Microsoft::WRL;

struct ID3D11Buffer;

namespace CU = CommonUtilities;

namespace FGE
{
	class Light
	{
	public:
		struct LightBufferData
		{
			CU::Vector3f Color = CU::Vector3f(1.0f, 1.0f, 1.0f);
			float Intensity = 1;
			alignas(16)CU::Vector3f Direction = CU::Vector3f(0.0f, 0.0f, 0.0f);
		};

	public:
		Light() = default;
		virtual ~Light() = default;
		void Init(const CU::Vector3f& aColor, float aIntensity);

		virtual void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) = 0;
		
	protected:
		LightBufferData myLightBufferData;
		
		FORCEINLINE CU::Vector3f GetColor() const { return myLightBufferData.Color; }
		FORCEINLINE float GetIntensity() const { return myLightBufferData.Intensity; }
	};
}