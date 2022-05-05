#pragma once
#include "FGE/Asset/Model.h"
#include "FGE/Asset/Vertex.h"
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
		static std::shared_ptr<Mesh> CreateModelFromVertices(const std::vector<Vertex>& someVertices, const std::vector<unsigned int>& someIndices, std::string aName);

		static std::unordered_map<std::string, std::shared_ptr<Mesh>> myAssets;
	};
}