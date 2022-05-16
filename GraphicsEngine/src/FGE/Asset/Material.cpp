#include "GraphicsEngine.pch.h"
#include "Material.h"

FGE::Material::Material()
{
	myAlbedo = { 0,0,0};
}

FGE::Material::~Material()
{
}

void FGE::Material::SetAlbedo(const CU::Vector3f& aAlbedo)
{
	myAlbedo = aAlbedo;
}
