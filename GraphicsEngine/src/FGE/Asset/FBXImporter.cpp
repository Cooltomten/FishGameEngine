#include "GraphicsEngine.pch.h"
#include "FBXImporter.h"

#include "FGE/Asset/AnimatedMesh.h"
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
		ExtractSubMeshes(tgaModel, subMeshes);

		mesh->Init(subMeshes);
		return mesh;
	}
	std::shared_ptr<AnimatedMesh> FBXImporter::ImportAnimatedMesh(const std::string& aPath)
	{
		TGA::FBXModel tgaModel;

		if (!TGA::FBXImporter::LoadModel(aPath, tgaModel))
		{
			//Boom!
		}

		std::vector<std::shared_ptr<SubMesh>> subMeshes;
		ExtractSubMeshes(tgaModel, subMeshes);

		//if (!TGA::FBXImporter::LoadSkeleton(aPath, tgaModel))
		//{
		//	//Boom!
		//}

		std::shared_ptr<AnimatedMesh> mesh = std::make_shared<AnimatedMesh>();

		Skeleton skeleton;
		skeleton.Name = tgaModel.Skeleton.Name;

		for (auto& tgaBone : tgaModel.Skeleton.Bones)
		{
			Skeleton::Bone bone;

			bone.BindPoseInverse = tgaBone.BindPoseInverse.Data;

			bone.Name = tgaBone.Name;
			bone.Children = tgaBone.Children;
			bone.Parent = tgaBone.Parent;
			skeleton.Bones.push_back(bone);
		}

		skeleton.BoneNameToIndex = tgaModel.Skeleton.BoneNameToIndex;

		mesh->Init(subMeshes, skeleton);

		return mesh;
	}
	std::shared_ptr<Animation> FBXImporter::ImportAnimation(const std::string& aPath)
	{
		TGA::FBXAnimation tgaAnimation;
		if (!TGA::FBXImporter::LoadAnimation(aPath, tgaAnimation))
		{
			//Boom!
		}

		std::shared_ptr<Animation> animation = std::make_shared<Animation>();

		animation->Name = tgaAnimation.Name;
		animation->FrameCount = tgaAnimation.Length;
		animation->Duration = tgaAnimation.Duration;
		animation->FramesPerSecond = tgaAnimation.FramesPerSecond;

		for (auto& tgaFrame : tgaAnimation.Frames)
		{
			Animation::Frame frame;
			for (auto& tgaLocTrans : tgaFrame.LocalTransforms)
			{
				CU::Matrix4x4<float> localTransform;
				localTransform = tgaLocTrans.Data;
				frame.LocalTransforms.push_back(localTransform);
			}
			animation->Frames.push_back(frame);
		}


		return animation;
	}
	void FBXImporter::ExtractSubMeshes(TGA::FBXModel& aFbxModel, std::vector<std::shared_ptr<SubMesh>>& aMeshes)
	{
		for (auto& subMesh : aFbxModel.Meshes)
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
			aMeshes.push_back(fgeSubMesh);
		}
	}
}
