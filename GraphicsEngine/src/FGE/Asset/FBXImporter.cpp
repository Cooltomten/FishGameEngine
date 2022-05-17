#include "GraphicsEngine.pch.h"
#include "FBXImporter.h"

#include "FGE/Asset/Mesh.h"
#include "FGE/Asset/SubMesh.h"

#include "FGE/Rendering/Buffers/Vertex.h"


#include <TgaFbxImporter/FBXImporter.h>


namespace FGE
{
	std::shared_ptr<Mesh> FBXImporter::ImportMesh(const std::string& aPath)
	{
		
		TGA::FBXModel tgaModel;
		if (!TGA::FBXImporter::LoadModel(aPath, tgaModel))
		{
			//Boom!
		}
		
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

		std::vector<std::shared_ptr<SubMesh>> subMeshes;

		for (auto& subMesh : tgaModel.Meshes)
		{
			std::vector<Vertex> vertices;

			for (auto& vertex : subMesh.Vertices)
			{
				vertex.VertexColors[0][0] = (rand() / static_cast<float>(RAND_MAX) + 1) / 2;
				vertex.VertexColors[0][1] = (rand() / static_cast<float>(RAND_MAX) + 1) / 2;
				vertex.VertexColors[0][2] = (rand() / static_cast<float>(RAND_MAX) + 1) / 2;
				vertex.VertexColors[0][3] = 1.0f;
				vertices.emplace_back(Vertex(vertex.Position, vertex.VertexColors, vertex.UVs, vertex.Normal,
											 vertex.Tangent, vertex.Binormal, vertex.BoneIDs, vertex.BoneWeights));
			}
			

			std::shared_ptr<SubMesh> fgeSubMesh = std::make_shared<SubMesh>(vertices, subMesh.Indices);
			subMeshes.push_back(fgeSubMesh);
		}

		mesh->Init(aPath, subMeshes);
		return mesh;
	}
}
