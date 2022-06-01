#pragma once
#include "FGE/Rendering/Lights/Light.h"

namespace FGE
{
	class DirectionalLight : public Light
	{
		void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) override;
	};
}