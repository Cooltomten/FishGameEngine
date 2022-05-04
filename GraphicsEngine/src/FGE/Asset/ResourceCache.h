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
		
		static void SaveAsset(const std::string& aName, std::shared_ptr<Model> aModel);

		static std::shared_ptr<Model> GetAsset(const std::string& aName);
		
	private:
		static std::shared_ptr<Model> CreateUnitCube();
		static std::shared_ptr<Model> CreateModelFromVertices(const std::vector<Vertex>& someVertices, const std::vector<unsigned int>& someIndices, std::string aName);

		static std::unordered_map<std::string, std::shared_ptr<Model>> myAssets;
	};
}