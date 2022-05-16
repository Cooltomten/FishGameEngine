#pragma once
#include "GraphicsEngine.pch.h"

namespace FGE
{
	class Mesh;
	class FBXImporter 
	{
	public:
		FBXImporter() = default;
		~FBXImporter() = default;
		
		std::shared_ptr<Mesh> ImportMesh(const std::string& aPath);
		
	};
}