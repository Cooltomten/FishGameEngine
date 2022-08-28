#pragma once
#include <memory>
#include <string>

namespace FGE
{
	class PixelShader;
	class VertexShader;
	class ShaderImporter
	{
		public:
		ShaderImporter() = default;
		~ShaderImporter() = default;

		//std::shared_ptr<PixelShader> ImportPixelShader(const std::string& fileName);
		//std::shared_ptr<VertexShader> ImportVertexShader(const std::string& fileName);

		
	};
}