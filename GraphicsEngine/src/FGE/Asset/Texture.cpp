#include "GraphicsEngine.pch.h"
#include "Texture.h"

#include "FGE/Core/Application.h"
#include "FGE/Core/DX11.h"

namespace FGE
{
	Texture::~Texture()
	{
		myTexture.Reset();
		mySRV.Reset();
	}
	void FGE::Texture::SetAsResource(uint32_t aSlot)
	{
		 Application::Get().GetWindow()->GetDX11().GetDeviceContext()->PSSetShaderResources(aSlot, 1, mySRV.GetAddressOf());
	}
}
