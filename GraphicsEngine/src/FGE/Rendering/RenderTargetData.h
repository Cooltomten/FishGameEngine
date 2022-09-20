#pragma once
#include <wrl.h>
#include <d3d11.h>
using namespace Microsoft::WRL;
namespace FGE
{
	struct RenderTargetData
	{
		ComPtr<ID3D11DepthStencilView> DepthStencilView;
		ComPtr<ID3D11RenderTargetView> RenderTargetView;
		D3D11_VIEWPORT Viewport = {};
	};

}