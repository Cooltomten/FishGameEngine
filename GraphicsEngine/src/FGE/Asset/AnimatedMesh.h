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

		void Render(const CU::Matrix4x4<float>& aTransform, std::shared_ptr<Animation> aAnimation, float aAnimationTime);

		void SetMaterial(std::shared_ptr<Material> aMaterial, uint32_t aIndex);
		std::shared_ptr<Material> GetMaterial(uint32_t aIndex);

		FORCEINLINE const Skeleton* GetSkeleton() const { return &mySkeleton; }

	private:

		Skeleton mySkeleton;
		std::vector<std::shared_ptr<SubMesh>> mySubMeshes;

		std::unordered_map<uint32_t, std::shared_ptr<Material>> myMaterials;
	};

}