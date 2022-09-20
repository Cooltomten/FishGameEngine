#include "GraphicsEngine.pch.h"
#include "GBuffer.h"

#include "FGE/Core/Window.h"
#include "FGE/Core/DX11.h"


void FGE::GBuffer::Init()
{
	Resize(static_cast<int>(FGE::Window::Get().GetWidth()), static_cast<int>(FGE::Window::Get().GetHeight()));
}

void FGE::GBuffer::SetAsResource(uint32_t aStartSlot)
{
	ID3D11ShaderResourceView* srvs[GBuffer::GB_COUNT];
	for (uint32_t i = 0; i < GBuffer::GB_COUNT; i++)
	{
		srvs[i] = myTextures[i].GetShaderResourceView().Get();
	}

	FGE::Window::Get().GetDX11().GetDeviceContext()->PSSetShaderResources(aStartSlot, GBuffer::GB_COUNT, &srvs[0]);
}

void FGE::GBuffer::SetAsTarget(ID3D11DepthStencilView* aDSV)

{
	ID3D11RenderTargetView* rtvs[GBuffer::GB_COUNT];
	for (uint32_t i = 0; i < GBuffer::GB_COUNT; i++)
	{
		rtvs[i] = myTextures[i].GetRenderTargetView().Get();
	}
	
	FGE::Window::Get().GetDX11().GetDeviceContext()->OMSetRenderTargets(GBuffer::GB_COUNT, &rtvs[0], aDSV);
	
}

void FGE::GBuffer::Clear()
{
	auto& dx11 = FGE::Window::Get().GetDX11();

	auto clearColor = dx11.GetClearColor();
	
	for (uint32_t i = 0; i < GBuffer::GB_COUNT; i++)
	{
		myTextures[i].ClearRenderTarget(dx11.GetDeviceContext(), clearColor[0], clearColor[1], clearColor[2], 0);
	}
}

void FGE::GBuffer::Resize(int aWidth, int aHeight)
{
	for (uint32_t i = 0; i < GB_COUNT; ++i)
	{
		myTextures[i].Initialize(FGE::Window::Get().GetDX11().GetDevice(), aWidth, aHeight);
	}
}
