#include "GraphicsEngine.pch.h"
#include "MaterialImporter.h"
#include "FGE/Asset/Material.h"
#include "FGE/Asset/ResourceCache.h"

#include <nlohmann/json.hpp>
#include <filesystem>

namespace FGE
{
	std::shared_ptr<Material> MaterialImporter::ImportMaterial(const std::filesystem::path& aPath)
	{
		std::shared_ptr<Material> material = std::make_shared<Material>();

		std::ifstream loadFile;
		loadFile.open(aPath);

		nlohmann::json doc;
		loadFile >> doc;

		std::string albedoPath = doc["AlbedoTexture"];
		std::string normalPath = doc["NormalTexture"];
		std::string materialPath = doc["MaterialTexture"];

		if (std::filesystem::exists(albedoPath))
		{
			material->SetTexture(MaterialTextureChannel::Albedo, ResourceCache::GetAsset<Texture>(albedoPath));
		}
		
		if (std::filesystem::exists(normalPath))
		{
			material->SetTexture(MaterialTextureChannel::Normal, ResourceCache::GetAsset<Texture>(normalPath));
		}

		if (std::filesystem::exists(materialPath))
		{
			material->SetTexture(MaterialTextureChannel::Material, ResourceCache::GetAsset<Texture>(materialPath));
		}


		return material;
	}
}