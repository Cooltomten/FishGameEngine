#pragma once

//for comptr
#include <wrl.h>
#include <d3d11.h>
using namespace Microsoft::WRL;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
namespace FGE
{
	class RenderTexture
	{
	public:
		RenderTexture();
		RenderTexture(const RenderTexture& other);
		~RenderTexture();

		bool Initialize(ComPtr<ID3D11Device> aDevice, int aTextureWith, int aTextureHeight);
		void Shutdown();
		
		void SetRenderTarget(ComPtr<ID3D11DeviceContext> aDeviceContext);
		void ClearRenderTarget(ComPtr<ID3D11DeviceContext> aDeviceContext, float aR, float aG, float aB, float aA);
		ComPtr<ID3D11ShaderResourceView> GetShaderResourceView();
		ComPtr<ID3D11RenderTargetView> GetRenderTargetView();
		ComPtr<ID3D11DepthStencilView> GetDepthStencilView();

	private:
		ComPtr<ID3D11Texture2D> myTexture;
		ComPtr<ID3D11DepthStencilView> myDepthStencilView;
		ComPtr<ID3D11RenderTargetView> myRenderTargetView;
		ComPtr<ID3D11ShaderResourceView> myShaderResourceView;	
		D3D11_VIEWPORT myViewport;
	};
}