#include "GraphicsEngine.pch.h"
#include "DX11.h"
#include "FGE/Core/DXErrorHandling/dxerr.h"
#include <sstream>

#define GFX_THROW_FAILED(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__, __FILE__, hr)
//#define GFX_DEVICE_REMOVED_EXCEPT(hr) DX11::DeviceRemovedException(__LINE__,__FILE__, hr)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) DX11::HrException( __LINE__,__FILE__,(hr),myInfoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) myInfoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) DX11::DeviceRemovedException( __LINE__,__FILE__,(hr),myInfoManager.GetMessages() )
#else
#define GFX_EXCEPT(hr) DX11::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) DX11::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#endif

namespace FGE
{

	DX11::DX11()
	{

		myClearColor = { 0.1,0.1,0.1,1 };
	}

	DX11::~DX11()
	{
	}

	bool DX11::Initialize(HWND aWindowHandle)
	{
		DXGI_SWAP_CHAIN_DESC sd = { };
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = aWindowHandle;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		UINT swapCreateFlags = 0u;

#ifndef NDEBUG
		swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		//for checking results of d3d functions
		HRESULT hr;

		GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			swapCreateFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&SwapChain,
			&Device,
			nullptr,
			&Context
		));


		//Gain access to texture subresource in swap chain (back buffer)
		ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
		GFX_THROW_INFO(SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(pBackBuffer.GetAddressOf())));
		GFX_THROW_INFO(Device->CreateRenderTargetView(
			pBackBuffer.Get(),
			nullptr,
			myRenderTargetData.RenderTargetView.GetAddressOf()
		));


		RECT clientRect = { 0,0,0,0 };
		GetClientRect(aWindowHandle, &clientRect);

		UINT width = clientRect.right - clientRect.left;
		UINT height = clientRect.bottom - clientRect.top;
		CreateDepthBuffer(width, height);
		SetRenderTarget();
		CreateViewport(static_cast<float>(width), static_cast<float>(height));
		Context->RSSetViewports(1, &myRenderTargetData.Viewport);
		return true;
	}

	void DX11::BeginFrame()
	{
		//clear the back buffer
		Context->ClearRenderTargetView(myRenderTargetData.RenderTargetView.Get(), &myClearColor[0]);
		Context->ClearDepthStencilView(myRenderTargetData.DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		myClearColorChangedFlag = false;
	}

	void DX11::EndFrame()
	{
		HRESULT hr;
#ifndef	NDEBUG
		myInfoManager.Set();
#endif
		if (FAILED(hr = SwapChain->Present(1u, 0u)))
		{
			if (hr == DXGI_ERROR_DEVICE_REMOVED)
			{
				throw GFX_DEVICE_REMOVED_EXCEPT(Device->GetDeviceRemovedReason());
			}
			else
			{
				throw GFX_EXCEPT(hr);
			}
		}
	}

	ID3D11Buffer* DX11::CreateBuffer(const D3D11_BUFFER_DESC* aDesc, const D3D11_SUBRESOURCE_DATA* aData)
	{
		ID3D11Buffer* buffer = nullptr;
		HRESULT hr;
		GFX_THROW_INFO(Device->CreateBuffer(aDesc, aData, &buffer));
		return buffer;
	}


	ID3D11VertexShader* DX11::CreateVertexShader(const char* aData, size_t aSize)
	{
		ID3D11VertexShader* shader = nullptr;
		HRESULT hr;
		GFX_THROW_INFO(Device->CreateVertexShader(aData, aSize, nullptr, &shader));

		return shader;
	}

	ID3D11GeometryShader* DX11::CreateGeometryShader(const char* aData, size_t aSize)
	{
		ID3D11GeometryShader* shader = nullptr;
		HRESULT hr;
		GFX_THROW_INFO(Device->CreateGeometryShader(aData, aSize, nullptr, &shader));

		return shader;
	}

	ID3D11PixelShader* DX11::CreatePixelShader(const char* aData, size_t aSize)
	{
		ID3D11PixelShader* shader = nullptr;
		HRESULT hr;
		GFX_THROW_INFO(Device->CreatePixelShader(aData, aSize, nullptr, &shader));
		return shader;
	}

	ID3D11InputLayout* DX11::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC aDesc[], UINT aSize, const char* aData, size_t aDataSize)
	{
		ID3D11InputLayout* layout = nullptr;
		HRESULT hr;
		GFX_THROW_INFO(Device->CreateInputLayout(aDesc, aSize, aData, aDataSize, &layout));
		return layout;
	}

	ComPtr<ID3D11DeviceContext>& DX11::GetDeviceContext()
	{
		return Context;
	}

	ComPtr<ID3D11Device>& DX11::GetDevice()
	{
		return Device;
	}

	ComPtr<IDXGISwapChain>& DX11::GetSwapChain()
	{
		return SwapChain;
	}

	ComPtr<ID3D11RenderTargetView>& DX11::GetRenderTargetView()
	{
		return myRenderTargetData.RenderTargetView;
	}

	ComPtr<ID3D11DepthStencilView>& DX11::GetDepthStencilView()
	{
		return myRenderTargetData.DepthStencilView;
	}

	void DX11::Map(ID3D11Resource* aResource, UINT aSubResource, D3D11_MAP aMapType, UINT aMapFlags, D3D11_MAPPED_SUBRESOURCE* aMappedResource)
	{
		HRESULT hr;
		GFX_THROW_INFO(Context->Map(aResource, aSubResource, aMapType, aMapFlags, aMappedResource));
	}

	void DX11::Unmap(ID3D11Resource* aResource, UINT aSubResource)
	{
		Context->Unmap(aResource, aSubResource);
	}

	void DX11::VSSetConstantBuffers(UINT aStartSlot, UINT aNumBuffers, ID3D11Buffer* const* aBuffers)
	{
		Context->VSSetConstantBuffers(aStartSlot, aNumBuffers, aBuffers);
	}

	void DX11::PSSetConstantBuffers(UINT aStartSlot, UINT aNumBuffers, ID3D11Buffer* const* aBuffers)
	{
		Context->PSSetConstantBuffers(aStartSlot, aNumBuffers, aBuffers);
	}

	void DX11::SetRenderTarget()
	{
		Context->OMSetRenderTargets(1, myRenderTargetData.RenderTargetView.GetAddressOf(), myRenderTargetData.DepthStencilView.Get());
		Context->RSSetViewports(1, &myRenderTargetData.Viewport);
	}

	void DX11::Resize(UINT aWidth, UINT aHeight)
	{

		myRenderTargetData.RenderTargetView.Reset();
		myRenderTargetData.DepthStencilView.Reset();

		SwapChain->ResizeBuffers(1, aWidth, aHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		HRESULT hr;


		ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
		GFX_THROW_INFO(SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(pBackBuffer.GetAddressOf())));
		D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
		GFX_THROW_INFO(Device->CreateRenderTargetView(
			pBackBuffer.Get(),
			nullptr,
			myRenderTargetData.RenderTargetView.GetAddressOf()
		));
		CreateDepthBuffer(aWidth, aHeight);
		Context->OMSetRenderTargets(1, myRenderTargetData.RenderTargetView.GetAddressOf(), myRenderTargetData.DepthStencilView.Get());
		CreateViewport(static_cast<float>(aWidth), static_cast<float>(aHeight));
		Context->RSSetViewports(1, &myRenderTargetData.Viewport);


	}

	void DX11::CreateDepthBuffer(UINT aWidth, UINT aHeight)
	{
		ComPtr<ID3D11Texture2D> depthBufferTexture;
		D3D11_TEXTURE2D_DESC depthBufferDesc = { 0 };
		depthBufferDesc.Width = aWidth;
		depthBufferDesc.Height = aHeight;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		HRESULT hr;
		GFX_THROW_INFO(Device->CreateTexture2D(&depthBufferDesc, nullptr, depthBufferTexture.GetAddressOf()));

		GFX_THROW_INFO(Device->CreateDepthStencilView(
			depthBufferTexture.Get(),
			nullptr,
			myRenderTargetData.DepthStencilView.GetAddressOf()
		));
	}

	void DX11::CreateViewport(float aWidth, float aHeight)
	{

		myRenderTargetData.Viewport.TopLeftX = 0;
		myRenderTargetData.Viewport.TopLeftY = 0;
		myRenderTargetData.Viewport.Width = aWidth;
		myRenderTargetData.Viewport.Height = aHeight;
		myRenderTargetData.Viewport.MinDepth = 0.0f;
		myRenderTargetData.Viewport.MaxDepth = 1.0f;

	}
	void DX11::SetClearColor(std::array<float, 4> aClearColor)
	{
		myClearColor = aClearColor;
		myClearColorChangedFlag = true;
	}
	bool DX11::ClearColorChanged()
	{
		return myClearColorChangedFlag;
	}





	//Graphics exception stuff

	DX11::HrException::HrException(int aLine, const char* aFile, HRESULT aHr, std::vector<std::string> someMessages) noexcept
		:Exception(aLine, aFile), myHr(aHr)
	{
		//join all info messages with newlines into single string
		for (const auto& m : someMessages)
		{
			myInfo += m;
			myInfo += "\n";
		}
		//remove final newline if exists
		if (!myInfo.empty())
		{
			myInfo.pop_back();
		}

	}

	const char* DX11::HrException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
			<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
			<< "[Error String] " << GetErrorString() << std::endl
			<< "[Description] " << GetErrorDescription() << std::endl;
		if (!myInfo.empty())
		{
			oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
		}
		oss << GetOriginString();

		myWhatBuffer = oss.str();
		return myWhatBuffer.c_str();
	}

	const char* DX11::HrException::GetType() const noexcept
	{
		return "Fabian Graphics Exception";
	}

	HRESULT DX11::HrException::GetErrorCode() const noexcept
	{
		return myHr;
	}

	std::string DX11::HrException::GetErrorString() const noexcept
	{
		return DXGetErrorStringA(myHr);
	}

	std::string DX11::HrException::GetErrorDescription() const noexcept
	{
		char buf[512];
		DXGetErrorDescriptionA(myHr, buf, sizeof(buf));
		return buf;
	}

	std::string DX11::HrException::GetErrorInfo() const noexcept
	{
		return myInfo;
	}

	const char* DX11::DeviceRemovedException::GetType() const noexcept
	{
		return "Fabian Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
	}

}
