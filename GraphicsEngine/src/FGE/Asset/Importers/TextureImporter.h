#pragma once

#include "GraphicsEngine.pch.h"

namespace FGE
{
	class Texture;
	class TextureImporter
	{
	public:
		
		std::shared_ptr<Texture> ImportTexture(const std::filesystem::path& filePath);
	};
}