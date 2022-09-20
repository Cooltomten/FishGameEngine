#pragma once
#include "FGE/Rendering/Lights/Light.h"

namespace FGE
{
	class SpotLight : public Light
	{
	public:
		SpotLight() = default;
		 ~SpotLight() override = default;

		static std::shared_ptr < SpotLight> Create(const CU::Vector3f& aColor, float aIntensity, float aRange, float aInnerCone, float aOuterCone);
		
		void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) override;

		void SetRange(float aRange);
		void SetInnerCone(float anAngle);
		void SetOuterCone(float anAngle);

		FORCEINLINE float GetRange() const { return myRange; }
		FORCEINLINE float GetInnerCone() const { return myInnerCone; }
		FORCEINLINE float GetOuterCone() const { return myOuterCone; }

	private:
		float myRange = 0.0f;
		float myInnerCone = 0.0f;
		float myOuterCone = 0.0f;
	};

}