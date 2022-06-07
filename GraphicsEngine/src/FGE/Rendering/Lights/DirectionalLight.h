#pragma once
#include "FGE/Rendering/Lights/Light.h"

namespace FGE
{
	class DirectionalLight : public Light
	{
	public:
		~DirectionalLight() override = default;
		void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) override;
		void SetDirection(CU::Vector3f aDirection);
	};
}