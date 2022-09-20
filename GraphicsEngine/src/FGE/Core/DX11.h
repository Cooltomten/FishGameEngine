#pragma once
#include "FabException.h"
#include "FGE/Core/DXErrorHandling/DxgiInfoManager.h"
#include "FGE/Rendering/RenderTargetData.h"
#include <vector>
#include <wrl.h>
#include <array>
#include <d3d11.h>
//for ComPtrs
using namespace Microsoft::WRL;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11SamplerState;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Buffer;
struct ID3D11Texture2D;

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Resource;

struct D3D11_INPUT_ELEMENT_DESC;
struct D3D11_BUFFER_DESC;
struct D3D11_MAPPED_SUBRESOURCE;
struct D3D11_SUBRESOURCE_DATA;
struct D3D11_VIEWPORT;

enum D3D11_MAP;

namespace FGE
{
	class DX11
	{
	public:
		class Exception : public FabException
		{
			using FabException::FabException;
		};
		class HrException : public Exception
		{
		public:
			HrException(int aLine, const char* aFile, HRESULT aHr, std::vector<std::string> someMessages = {}) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			HRESULT GetErrorCode() const noexcept;
			std::string GetErrorString() const noexcept;
			std::string GetErrorDescription() const noexcept;
			std::string GetErrorInfo() const noexcept;
		private:
			HRESULT myHr;
			std::string myInfo;
		};
		class DeviceRemovedException :public HrException
		{
			using HrException::HrException;
		public:
			const char* GetType() const noexcept override;
		private:
			std::string myReason;
		};


	public:
		DX11();
		~DX11();


		bool Initialize(HWND aWindowHandle);

		void BeginFrame();
		void EndFrame();
		bool ClearColorChanged();
		ID3D11Buffer* CreateBuffer(const D3D11_BUFFER_DESC* aDesc, const D3D11_SUBRESOURCE_DATA* aData);

		ID3D11VertexShader* CreateVertexShader(const char* aData, size_t aSize);
		ID3D11GeometryShader* CreateGeometryShader(const char* aData, size_t aSize);
		ID3D11PixelShader* CreatePixelShader(const char* aData, size_t aSize);
		ID3D11InputLayout* CreateInputLayout(D3D11_INPUT_ELEMENT_DESC aDesc[], UINT aSize, const char* aData, size_t aDataSize);

		ComPtr<ID3D11DeviceContext>& GetDeviceContext();
		ComPtr<ID3D11Device>& GetDevice();
		ComPtr<IDXGISwapChain>& GetSwapChain();
		ComPtr<ID3D11RenderTargetView>& GetRenderTargetView();
		ComPtr<ID3D11DepthStencilView>& GetDepthStencilView();

		void Map(ID3D11Resource* aResource, UINT aSubResource, D3D11_MAP aMapType, UINT aMapFlags, D3D11_MAPPED_SUBRESOURCE* aMappedResource);
		void Unmap(ID3D11Resource* aResource, UINT aSubResource);
		void VSSetConstantBuffers(UINT aStartSlot, UINT aNumBuffers, ID3D11Buffer* const* aBuffers);
		void PSSetConstantBuffers(UINT aStartSlot, UINT aNumBuffers, ID3D11Buffer* const* aBuffers);

#ifndef NDEBUG
		inline DxgiInfoManager& GetInfoManager() { return myInfoManager; }
#endif
		void SetRenderTarget();
		std::array<float, 4> GetClearColor() const { return myClearColor; }
		void SetClearColor(std::array<float, 4> aClearColor);

		inline const RenderTargetData& GetRenderTargetData() const { return myRenderTargetData; }

		void Resize(UINT aWidth, UINT aHeight);

	private:
		void CreateDepthBuffer(UINT aWidth, UINT aHeight);
		void CreateViewport(float aWidth, float aHeight);
		

		std::array<float, 4> myClearColor;
		ComPtr<ID3D11Device> Device;
		ComPtr<ID3D11DeviceContext> Context;
		ComPtr<IDXGISwapChain> SwapChain;

		ComPtr<ID3D11SamplerState> SamplerStateDefault;
		ComPtr<ID3D11SamplerState> SamplerStateWrap;

		//ComPtr<ID3D11RenderTargetView> BackBuffer;
		//ComPtr<ID3D11DepthStencilView> DepthBuffer;

		//D3D11_VIEWPORT myViewport;
		
		RenderTargetData myRenderTargetData;

		bool myClearColorChangedFlag = false;

#ifndef NDEBUG
		DxgiInfoManager myInfoManager;
#endif
	};
}




