#pragma once
#include "GraphicsEngine.pch.h"
#include <filesystem>

namespace FGE
{
	class Material;
	class MaterialImporter
	{
	public:
		MaterialImporter() = default;
		~MaterialImporter() = default;
		std::shared_ptr<Material> ImportMaterial(const std::filesystem::path& aPath);
	};
}