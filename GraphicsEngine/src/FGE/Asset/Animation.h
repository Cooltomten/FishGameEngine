#pragma once
#include "GraphicsEngine.pch.h"

#include "FGE/Asset/Asset.h"

#include "DirectXTK/SimpleMath.h"

#include <CommonUtilities/Math/Matrix4x4.hpp>
namespace CU = CommonUtilities;

namespace FGE
{
	class Skeleton;
	class Animation : public Asset
	{
	public:
		Animation() = default;
		~Animation() = default;

		static AssetType GetStaticType() { return AssetType::Animation; }
		inline AssetType GetAssetType() const override { return GetStaticType(); }

		struct Frame
		{
			struct FrameTransform
			{
				DirectX::SimpleMath::Vector3 Translation;
				DirectX::SimpleMath::Quaternion Rotation;
				DirectX::SimpleMath::Vector3 Scale;
			};
			std::vector<FrameTransform> LocalTransforms;
		};

		std::vector<CU::Matrix4x4<float>> Sample(float aTime, Skeleton& aSkeleton, std::shared_ptr<Animation> aAnimationToBlendWith, float aBlendAlpha) const;
		float GetDuration() const;
		
		std::vector<Frame> Frames;

		unsigned int FrameCount;
		float FramesPerSecond;
		std::string Name;

	private:
		void CalculateTransforms(uint32_t aCurrentFrame,uint32_t aNextFrame,uint32_t aCurrentFrameBlend, uint32_t aNextFrameBlend,
			float aDelta, float aDeltaBlend,float aBlendAlpha, Skeleton& aSkeleton, uint32_t aIndex, const CU::Matrix4x4<float>& aParentTransform,
			const std::vector<Frame>& aFramesBlend,std::vector<CU::Matrix4x4<float>>& aTransforms) const;

	};
}