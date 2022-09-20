#pragma once
#include "FGE/Rendering/Lights/Light.h"

namespace FGE

{
	class PointLight : public Light
	{
	public:
		~PointLight() override = default;

		static std::shared_ptr < PointLight> Create(const CU::Vector3f& aColor, float aIntensity, float aRange);
		
		void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) override;

		void SetRange(float aRange);
		FORCEINLINE float GetRange() const { return myRange; }

	private:
		float myRange = 0.0f;
	};
		
}
