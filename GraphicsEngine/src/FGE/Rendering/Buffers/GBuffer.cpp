#include "GraphicsEngine.pch.h"
#include "GBuffer.h"
#include "FGE/Core/Application.h"

void FGE::GBuffer::Init()
{
	for (uint32_t i = 0; i < GB_COUNT; ++i)
	{
		myTextures[i].Initialize(FGE::Application::Get().GetWindow()->GetDX11().GetDevice(), Application::Get().GetWindow()->GetWidth(), Application::Get().GetWindow()->GetHeight());
	}
}

void FGE::GBuffer::SetAsResource(uint32_t aStartSlot)
{
	ID3D11ShaderResourceView* srvs[GBuffer::GB_COUNT];
	for (uint32_t i = 0; i < GBuffer::GB_COUNT; i++)
	{
		srvs[i] = myTextures[i].GetShaderResourceView().Get();
	}

	FGE::Application::Get().GetWindow()->GetDX11().GetDeviceContext()->PSSetShaderResources(aStartSlot, GBuffer::GB_COUNT, &srvs[0]);
}

void FGE::GBuffer::SetAsTarget(ID3D11DepthStencilView* aDSV)

{
	ID3D11RenderTargetView* rtvs[GBuffer::GB_COUNT];
	for (uint32_t i = 0; i < GBuffer::GB_COUNT; i++)
	{
		rtvs[i] = myTextures[i].GetRenderTargetView().Get();
	}
	
	FGE::Application::Get().GetWindow()->GetDX11().GetDeviceContext()->OMSetRenderTargets(GBuffer::GB_COUNT, &rtvs[0], aDSV);
	
}

void FGE::GBuffer::Clear()
{
	auto& dx11 = FGE::Application::Get().GetWindow()->GetDX11();

	auto clearColor = dx11.GetClearColor();
	
	for (uint32_t i = 0; i < GBuffer::GB_COUNT; i++)
	{
		myTextures[i].ClearRenderTarget(dx11.GetDeviceContext(), clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	}
}
