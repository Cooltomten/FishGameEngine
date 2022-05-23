#pragma once
#include "GraphicsEngine.pch.h"

#include "FGE/Asset/Asset.h"

#include <CommonUtilities/Math/Matrix4x4.hpp>
namespace CU = CommonUtilities;

namespace FGE
{
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

		std::vector<Frame> Frames;

		unsigned int FrameCount;
		float Duration;
		float FramesPerSecond;
		std::string Name;

	};
}