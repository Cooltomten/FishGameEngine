#include "GraphicsEngine.pch.h"
#include "SpotLight.h"

std::shared_ptr<FGE::SpotLight> FGE::SpotLight::Create(const CU::Vector3f& aColor, float aIntensity, float aRange, float aInnerCone, float aOuterCone)
{
	std::shared_ptr<FGE::SpotLight> result = std::make_shared<FGE::SpotLight>();
	result->Init(aColor, aIntensity);
	result->SetRange(aRange);
	result->SetInnerCone(aInnerCone);
	result->SetOuterCone(aOuterCone);
	
	return result;
}

void FGE::SpotLight::SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer)
{
}

void FGE::SpotLight::SetRange(float aRange)
{
	myRange = aRange;
}

void FGE::SpotLight::SetInnerCone(float anAngle)
{
	myInnerCone = anAngle;
}

void FGE::SpotLight::SetOuterCone(float anAngle)
{
	myOuterCone = anAngle;
}
