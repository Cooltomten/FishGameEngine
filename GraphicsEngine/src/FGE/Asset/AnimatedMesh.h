#pragma once
#include "GraphicsEngine.pch.h"

#include "Fge/Asset/Asset.h"
#include "FGE/Asset/Animation.h"

#include <CommonUtilities/Math/Matrix4x4.hpp>

namespace CU = CommonUtilities;

namespace FGE
{

	struct Skeleton
	{
		std::string Name;

		struct Bone
		{
			CU::Matrix4x4<float> BindPoseInverse;
			int Parent;
			std::vector<unsigned int> Children;
			std::string Name;
		};

		std::vector<Bone> Bones;
		std::unordered_map<std::string, size_t> BoneNameToIndex;

	};
	
	class SubMesh;
	class Material;
	class AnimatedMesh : public Asset
	{
	public:
		AnimatedMesh() = default;
		~AnimatedMesh() = default;
		static AssetType GetStaticType() { return AssetType::AnimatedMesh; }
		inline AssetType GetAssetType() const override { return GetStaticType(); }

		void Init(std::vector<std::shared_ptr<SubMesh>> aSubMesh, Skeleton& aSkeleton);

		void Render(const CU::Matrix4x4<float>& aTransform, const std::vector<std::shared_ptr<Material>>& someMaterials, std::shared_ptr<Animation> aAnimation,
			std::shared_ptr<Animation> aAnimationToBlendWith, float aBlendAlpha, float aAnimationTime);

		FORCEINLINE const Skeleton* GetSkeleton() const { return &mySkeleton; }

	private:

		Skeleton mySkeleton;
		std::vector<std::shared_ptr<SubMesh>> mySubMeshes;
	};

}