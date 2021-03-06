#pragma once
#include "FGE/Asset/Mesh.h"
#include "FGE/Asset/Texture.h"
#include "FGE/Rendering/Buffers/Vertex.h"

#include "FGE/Asset/FBXImporter.h"
#include "FGE/Asset/Importers/TextureImporter.h"

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
	};

	template<class T>
	inline std::shared_ptr<T> ResourceCache::GetAsset(const std::string& aPath)
	{
		auto it = myAssets.find(aPath);
		if (it != myAssets.end() && it->second->GetAssetType() == T::GetStaticType())
		{
			return std::reinterpret_pointer_cast<T>(it->second);
		}

		SaveAsset(ImportAsset <T>(aPath));


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
		}


		if (asset)
		{
			asset->SetPath(aPath);
		}
		return  std::reinterpret_pointer_cast<T>(asset);
	}

}