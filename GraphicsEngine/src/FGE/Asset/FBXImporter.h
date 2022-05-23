#pragma once
#include "GraphicsEngine.pch.h"

namespace TGA
{
	struct FBXModel;
}

namespace FGE
{
	class Mesh;
	class AnimatedMesh;
	class SubMesh;
	class Animation;
	class FBXImporter 
	{
	public:
		FBXImporter() = default;
		~FBXImporter() = default;
		
		std::shared_ptr<Mesh> ImportMesh(const std::string& aPath);
		std::shared_ptr<AnimatedMesh> ImportAnimatedMesh(const std::string& aPath);
		std::shared_ptr<Animation> ImportAnimation(const std::string& aPath);

	private:
		void ExtractSubMeshes(TGA::FBXModel& aFbxModel, std::vector<std::shared_ptr<SubMesh>>& aMeshes);
		
	};
}