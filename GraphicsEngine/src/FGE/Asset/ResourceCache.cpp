#include "GraphicsEngine.pch.h"
#include "ResourceCache.h"
#include "FGE/Core/Application.h"

#include "FGE/Rendering/Buffers/VertexBuffer.h"
#include "FGE/Rendering/Buffers/VertexArray.h"
#include "FGE/Rendering/Renderer.h"
#include "FGE/Asset/SubMesh.h"


namespace FGE
{
	std::unordered_map<std::string, std::shared_ptr<Asset>> ResourceCache::myAssets;
	std::unique_ptr<FBXImporter> ResourceCache::myFBXImporter;


	void ResourceCache::Initialize()
	{
		SaveAsset(CreateUnitCube());
	}

	void ResourceCache::SaveAsset(std::shared_ptr<Asset> aAsset)
	{
		myAssets[aAsset->GetPath().string()] = aAsset;
	}



	std::shared_ptr<Mesh> ResourceCache::CreateUnitCube()
	{
		const float s = 50.f;
		std::vector<Vertex> mdlVertices =
		{
			{
				{s,s,s,1},
				{1,0,0,1}
			},
			{
				{s,s,-s, 1},
				{0,1,0,1}
			},
			{
				{s,-s,s,0},
				{0,0,1,1}
			},
			{
				{s, -s, -s, 1},
				{0,1,1,1}
			},
			{
				{-s,s,-s,1},
				{1,1,0,1}
			},
			{
				{-s, s, s,1},
				{1,0,1,1}
			},
			{
				{-s,-s,-s,1},
				{0,0,0,1}
			},
			{
				{-s, -s, s,1},
				{1,1,1,1}
			}
		};
		std::vector<unsigned int> mdlIndices =
		{
			0,2,1,
			1,2,3,
			1,3,4,
			4,3,6,
			4,6,5,
			5,6,7,
			5,7,0,
			0,7,2,
			2,7,3,
			3,7,6,
			5,0,1,
			4,5,1
		};


		std::shared_ptr<SubMesh> subMesh = std::make_shared<SubMesh>(mdlVertices, mdlIndices);


		std::shared_ptr<Mesh> mdl = std::make_shared<Mesh>();

		mdl->Init({ subMesh });


		mdl->SetPath("Cube");
		return mdl;
	}


}