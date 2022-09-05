#pragma once
#include "FGE/Asset/Mesh.h"
#include "FGE/Asset/Texture.h"
#include "FGE/Rendering/Buffers/Vertex.h"

#include "FGE/Asset/Importers/FBXImporter.h"
#include "FGE/Asset/Importers/TextureImporter.h"
#include "FGE/Asset/Importers/EmitterSettingsImporter.h"
#include "FGE/Asset/Importers/MaterialImporter.h"

#include "GraphicsEngine.pch.h"

#include "FGE/Asset/Asset.h"

namespace FGE
{
	class ResourceCache
	{
	public:
		static void Initialize();

		static void SaveAsset(std::shared_ptr<Asset> aAsset);

		template <class T>
		static std::shared_ptr<T> GetAsset(const std::string& aPath);

	private:
		template <class T>
		static std::shared_ptr<T> ImportAsset(std::filesystem::path aPath);

		static std::shared_ptr<Mesh> CreateUnitCube();

		static std::unordered_map<std::string, std::shared_ptr<Asset>> myAssets;

		static std::unique_ptr<FBXImporter> myFBXImporter;
		static std::unique_ptr<TextureImporter> myTextureImporter;
		static std::unique_ptr<EmitterSettingsImporter> myParticleEmitterImporter;
		//static std::unique_ptr<ShaderImporter> myShaderImporter;
		static std::unique_ptr<MaterialImporter> myMaterialImporter;
	};

	template<class T>
	inline std::shared_ptr<T> ResourceCache::GetAsset(const std::string& aPath)
	{
		auto it = myAssets.find(aPath);
		if (it != myAssets.end() && it->second->GetAssetType() == T::GetStaticType())
		{
			return std::reinterpret_pointer_cast<T>(it->second);
		}

		auto asset = ImportAsset <T>(aPath);
		if (asset)
		{
			SaveAsset(asset);
		}
		else
		{
			return std::shared_ptr<T>();
		}


		return std::reinterpret_pointer_cast<T>(myAssets[aPath]);
	}

	template<class T>
	inline std::shared_ptr<T> ResourceCache::ImportAsset(std::filesystem::path aPath)
	{
		std::shared_ptr<Asset> asset;
		switch (T::GetStaticType())
		{
		case AssetType::Mesh:
			asset = std::reinterpret_pointer_cast<Asset>(myFBXImporter->ImportMesh(aPath.string()));
			break;
		case AssetType::AnimatedMesh:
			asset = std::reinterpret_pointer_cast<Asset>(myFBXImporter->ImportAnimatedMesh(aPath.string()));
			break;
		case AssetType::Animation:
			asset = std::reinterpret_pointer_cast<Asset>(myFBXImporter->ImportAnimation(aPath.string()));
			break;
		case AssetType::Texture:
			asset = std::reinterpret_pointer_cast<Asset>(myTextureImporter->ImportTexture(aPath));
			break;
		case AssetType::PixelShader:
			//asset = std::reinterpret_pointer_cast<Asset>(myShaderImporter->ImportPixelShader(aPath));
			break;
		case AssetType::EmitterSettingsData:
			asset = std::reinterpret_pointer_cast<Asset>(myParticleEmitterImporter->ImportEmitterSettings(aPath));
			break;
		case AssetType::Material:
			asset = std::reinterpret_pointer_cast<Asset>(myMaterialImporter->ImportMaterial(aPath));
			break;

		}


		if (asset)
		{
			asset->SetPath(aPath);
		}
		return  std::reinterpret_pointer_cast<T>(asset);
	}

}