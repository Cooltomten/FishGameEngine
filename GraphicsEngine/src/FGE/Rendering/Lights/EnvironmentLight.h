#pragma once
#include "FGE/Rendering/Lights/Light.h"
#include <memory>

namespace FGE
{
	class Texture;
	class EnvironmentLight : public Light
	{
	public:
		void SetCubeMapTexture(std::shared_ptr<Texture> aTexture);
		virtual void SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer) override;

	private:
		std::shared_ptr<Texture> myTexture;
	};
}