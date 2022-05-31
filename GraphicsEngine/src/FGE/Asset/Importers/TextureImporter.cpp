#include "GraphicsEngine.pch.h"
#include "TextureImporter.h"

#include "FGE/Asset/Texture.h"
#include "FGE/Core/Application.h"
#include "FGE/Core/DX11.h"

#include "DirectXTK/DDSTextureLoader11.h"

#include <iostream>

namespace FGE
{
	std::shared_ptr<Texture> FGE::TextureImporter::ImportTexture(const std::filesystem::path& filePath)
	{
		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		
		HRESULT hr = DirectX::CreateDDSTextureFromFile(Application::Get().GetWindow()->GetDX11().GetDevice().Get(),
			filePath.wstring().c_str(), texture->myTexture.GetAddressOf(), texture->mySRV.GetAddressOf());

		if (FAILED(hr))
		{
			std::cout << "Couldnt load texture at path: " << filePath << std::endl;
		}

		return texture;
	}
}