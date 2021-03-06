#include "GraphicsEngine.pch.h"
#include "Material.h"

#include "FGE/Asset/Texture.h"

#include "FGE/Core/Application.h"
#include "FGE/Core/DX11.h"

std::shared_ptr<FGE::Material> FGE::Material::Default;

FGE::Material::Material()
{
	myMaterialData.Albedo = { 1,1,1 };
}

FGE::Material::~Material()
{
}

void FGE::Material::Init()
{
}

void FGE::Material::SetAsResource(ComPtr<ID3D11Resource> aMaterialBuffer)
{
	
	//map Material buffer
	auto& dx11 = Application::Get().GetWindow()->GetDX11();
	D3D11_MAPPED_SUBRESOURCE bufferData;
	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

	dx11.Map(aMaterialBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	memcpy(bufferData.pData, &myMaterialData, sizeof(MaterialData));
	dx11.Unmap(aMaterialBuffer.Get(), 0);

	if (myTextures[MaterialTextureChannel::Albedo])
	{
		myTextures[MaterialTextureChannel::Albedo]->SetAsResource(0);
	}
	if (myTextures[MaterialTextureChannel::Normal])
	{
		myTextures[MaterialTextureChannel::Normal]->SetAsResource(1);
	}
	
}

void FGE::Material::SetTexture(uint32_t aMaterialTextureChannel, std::shared_ptr<Texture> aTexture)
{
	myTextures[aMaterialTextureChannel] = aTexture;
}

void FGE::Material::SetAlbedo(const CU::Vector3f& aAlbedo)
{
	myMaterialData.Albedo = aAlbedo;
}
