#pragma once
#include "GraphicsEngine.pch.h"

#include "FGE/Asset/Asset.h"

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
			std::vector<CU::Matrix4x4<float>> LocalTransforms;
		};

		std::vector<CU::Matrix4x4<float>> Sample(float aTime, Skeleton& aSkeleton) const;

		std::vector<Frame> Frames;

		unsigned int FrameCount;
		float Duration;
		float FramesPerSecond;
		std::string Name;

	private:
		void CalculateTransforms(uint32_t aCurrentFrame, Skeleton& aSkeleton, uint32_t aIndex, const CU::Matrix4x4<float>& aParentTransform, std::vector<CU::Matrix4x4<float>>& aTransforms) const;

	};
}