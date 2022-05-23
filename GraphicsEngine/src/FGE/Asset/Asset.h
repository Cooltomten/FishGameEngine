#pragma once

#include "GraphicsEngine.pch.h"

namespace FGE
{
	enum class AssetType
	{
		Mesh,
		AnimatedMesh,
		Animation
	};

	class Asset
	{
	public:
		Asset() = default;
		virtual ~Asset() = default;
		virtual AssetType GetAssetType() const = 0;

		inline const std::filesystem::path& GetPath() const { return myPath; }
		inline void SetPath(const std::filesystem::path& aPath) { myPath = aPath; }
		

	protected:
		std::filesystem::path myPath;
	};
}