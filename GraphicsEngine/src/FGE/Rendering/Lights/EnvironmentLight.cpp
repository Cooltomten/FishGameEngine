#include "GraphicsEngine.pch.h"
#include "EnvironmentLight.h"

#include "FGE/Asset/Texture.h"

namespace FGE
{
	void EnvironmentLight::SetCubeMapTexture(std::shared_ptr<Texture> aTexture)
	{
		myTexture = aTexture;
	}
	void EnvironmentLight::SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer)
	{
		if (myTexture)
		{
			myTexture->SetAsResource(10);
		}
	}
}
