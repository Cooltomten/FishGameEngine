#include "GraphicsEngine.pch.h"
#include "PointLight.h"

std::shared_ptr<FGE::PointLight> FGE::PointLight::Create(const CU::Vector3f& aColor, float aIntensity, float aRange)
{
	std::shared_ptr<FGE::PointLight> result = std::make_shared<FGE::PointLight>();
	result->Init(aColor, aIntensity);
	result->SetRange(aRange);
	return result;
}

void FGE::PointLight::SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer)
{
}

void FGE::PointLight::SetRange(float aRange)
{
	myRange = aRange;
}
