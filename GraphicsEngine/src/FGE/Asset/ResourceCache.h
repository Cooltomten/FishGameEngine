#pragma once
#include "FGE/Asset/Mesh.h"
#include "FGE/Rendering/Buffers/Vertex.h"
#include "FGE/Asset/FBXImporter.h"
#include <memory>
#include <unordered_map>
#include <string>
namespace FGE
{
	class ResourceCache
	{
	public:
		static void Initialize();
		
		static void SaveAsset(const std::string& aName, std::shared_ptr<Mesh> aModel);

		static std::shared_ptr<Mesh> GetAsset(const std::string& aName);
		
	private:
		static std::shared_ptr<Mesh> CreateUnitCube();
		
		static std::unordered_map<std::string, std::shared_ptr<Mesh>> myAssets;

		static std::unique_ptr<FBXImporter> myFBXImporter;
	};
}