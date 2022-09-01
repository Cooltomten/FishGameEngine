#include "GraphicsEngine.pch.h"
#include "ResourceCache.h"

#include "FGE/Rendering/Buffers/VertexBuffer.h"
#include "FGE/Rendering/Buffers/VertexArray.h"
#include "FGE/Rendering/Renderer.h"
#include "FGE/Asset/SubMesh.h"
#include "FGE/Asset/Material.h"

namespace FGE
{
	std::unordered_map<std::string, std::shared_ptr<Asset>> ResourceCache::myAssets;
	std::unique_ptr<FBXImporter> ResourceCache::myFBXImporter;
	std::unique_ptr<TextureImporter> ResourceCache::myTextureImporter;
	std::unique_ptr<EmitterSettingsImporter> ResourceCache::myParticleEmitterImporter;
	

	void ResourceCache::Initialize()
	{
		Material::Default = std::make_shared<Material>();
		Material::Default->SetTexture(MaterialTextureChannel::Albedo, GetAsset<Texture>("Assets/Textures/T_Default_C.dds"));
		Material::Default->SetTexture(MaterialTextureChannel::Normal, GetAsset<Texture>("Assets/Textures/T_Default_N.dds"));
		Material::Default->SetTexture(MaterialTextureChannel::Material, GetAsset<Texture>("Assets/Textures/T_Default_M.dds"));
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
			//Top
			{
				{-s,s,s,1},
				{1,0,0,1},
				{0,0}
			},
			{
				{s,s,s,1},
				{1,0,0,1},
				{1,0},
			},
			{
				{s,s,-s,1},
				{1,0,0,1},
				{1,1},
			},
			{
				{-s,s,-s,1},
				{1,0,0,1},
				{0,1},
			},

			//Bottom
			{
				{-s,-s,s,1},
				{1,0,0,1},
				{0,0},
			},
			{
				{s,-s,s,1},
				{1,0,0,1},
				{1,0},
			},
			{
				{s,-s,-s,1},
				{1,0,0,1},
				{1,1},
			},
			{
				{-s,-s,-s,1},
				{1,0,0,1},
				{0,1},
			},

			//Positive Z
			{
				{-s,s,s,1},
				{1,0,0,1},
				{1,0}
			},
			{
				{s,s,s,1},
				{1,0,0,1},
				{0,0}
			},
			{
				{s,-s,s,1},
				{1,0,0,1},
				{0,1}
			},
			{
				{-s,-s,s,1},
				{1,0,0,1},
				{1,1}
			},

			//Negative Z
			{
				{-s,s,-s,1},
				{1,0,0,1},
				{0,0}
			},
			{
				{s,s,-s,1},
				{1,0,0,1},
				{1,0}
			},
			{
				{s,-s,-s,1},
				{1,0,0,1},
				{1,1}
			},
			{
				{-s,-s,-s,1},
				{1,0,0,1},
				{0,1}
			},

			//Positive X
			{
				{s,s,-s,1},
				{1,0,0,1},
				{0,0}
			},
			{
				{s,s,s,1},
				{1,0,0,1},
				{1,0}
			},
			{
				{s,-s,s,1},
				{1,0,0,1},
				{1,1}
			},
			{
				{s,-s,-s,1},
				{1,0,0,1},
				{0,1}
			},

			//Negative X
			{
				{-s,s,s,1},
				{1,0,0,1},
				{0,0}
			},
			{
				{-s,s,-s,1},
				{1,0,0,1},
				{1,0}
			},
			{
				{-s,-s,-s,1},
				{1,0,0,1},
				{1,1}
			},
			{
				{-s,-s,s,1},
				{1,0,0,1},
				{0,1}
			}
		};

		for (int i = 0; i < mdlVertices.size(); ++i)
		{
			//top
			if (i < 4)
			{
				mdlVertices[i].Normal = { 0, 1, 0 };//normal
				mdlVertices[i].Tangent = { 1, 0, 0 };//tangent
			}
			//bottom
			else if (i < 8)
			{
				mdlVertices[i].Normal = { 0, -1, 0 };//normal
				mdlVertices[i].Tangent = { -1, 0, 0 };//tangent
			}
			//positive z
			else if (i < 12)
			{
				mdlVertices[i].Normal = { 0, 0, 1 };//normal
				mdlVertices[i].Tangent = { 0, 1, 0 };//tangent
			}
			//negative z
			else if (i < 16)
			{
				mdlVertices[i].Normal = { 0, 0, -1 };//normal
				mdlVertices[i].Tangent = { 0, -1, 0 };//tangent
			}
			//positive x
			else if (i < 20)
			{
				mdlVertices[i].Normal = { 1, 0, 0 };//normal
				mdlVertices[i].Tangent = {0, -1, 0 };//tangent
			}
			//negative x
			else if (i < 24)
			{
				mdlVertices[i].Normal = { -1, 0, 0 };//normal
				mdlVertices[i].Tangent = { 0, 1, 0 };//tangent
			}

			mdlVertices[i].Binormal = mdlVertices[i].Tangent.Cross(mdlVertices[i].Normal);
		}
		std::vector<unsigned int> mdlIndices =
		{
			//top
			0,1,2,
			0,2,3,

			//bottom
			4,6,5,
			4,7,6,

			//positive z
			8,10,9,
			8,11,10,

			//negative z
			12,13,14,
			12,14,15,

			//positive x
			16,17,18,
			16,18,19,

			//negative x
			20,21,22,
			20,22,23

		};


		std::shared_ptr<SubMesh> subMesh = std::make_shared<SubMesh>(mdlVertices, mdlIndices);


		std::shared_ptr<Mesh> mdl = std::make_shared<Mesh>();

		mdl->Init({ subMesh });


		mdl->SetPath("Cube");
		return mdl;
	}


}