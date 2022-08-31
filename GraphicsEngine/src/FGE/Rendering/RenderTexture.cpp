#include "GraphicsEngine.pch.h"
#include "RenderTexture.h"


FGE::RenderTexture::RenderTexture()
{
}

FGE::RenderTexture::RenderTexture(const RenderTexture& other)
{
}

FGE::RenderTexture::~RenderTexture()
{
}

bool FGE::RenderTexture::Initialize(ComPtr<ID3D11Device> aDevice, int aTextureWidth, int aTextureHeight)
{

	myShaderResourceView.Reset();
	myTexture.Reset();
	myRenderTargetData.RenderTargetView.Reset();
	myRenderTargetData.DepthStencilView.Reset();
	
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hr;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	textureDesc.Width = aTextureWidth;
	textureDesc.Height = aTextureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	hr = aDevice->CreateTexture2D(&textureDesc, NULL, myTexture.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	ComPtr<ID3D11Texture2D> depthBufferTexture;
	D3D11_TEXTURE2D_DESC depthBufferDesc = { 0 };
	depthBufferDesc.Width = aTextureWidth;
	depthBufferDesc.Height = aTextureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hr = aDevice->CreateTexture2D(&depthBufferDesc, nullptr, depthBufferTexture.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	hr = aDevice->CreateDepthStencilView(
		depthBufferTexture.Get(),
		nullptr,
		myRenderTargetData.DepthStencilView.GetAddressOf()
	);
	if (FAILED(hr))
	{
		return false;
	}


	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	hr = aDevice->CreateRenderTargetView(myTexture.Get(), &renderTargetViewDesc, myRenderTargetData.RenderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	hr = aDevice->CreateShaderResourceView(myTexture.Get(), &shaderResourceViewDesc, myShaderResourceView.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	myRenderTargetData.Viewport.TopLeftX = 0;
	myRenderTargetData.Viewport.TopLeftY = 0;
	myRenderTargetData.Viewport.Width = static_cast<float>(aTextureWidth);
	myRenderTargetData.Viewport.Height = static_cast<float>(aTextureHeight);
	myRenderTargetData.Viewport.MinDepth = 0.0f;
	myRenderTargetData.Viewport.MaxDepth = 1.0f;

	return true;
}

void FGE::RenderTexture::Shutdown()
{

}

void FGE::RenderTexture::SetRenderTarget(ComPtr<ID3D11DeviceContext> aDeviceContext)
{
	aDeviceContext->OMSetRenderTargets(1, myRenderTargetData.RenderTargetView.GetAddressOf(), myRenderTargetData.DepthStencilView.Get());
	aDeviceContext->RSSetViewports(1, &myRenderTargetData.Viewport);
}

void FGE::RenderTexture::ClearRenderTarget(ComPtr<ID3D11DeviceContext> aDeviceContext, float aR, float aG, float aB, float aA)
{
	float color[4];
	color[0] = aR;
	color[1] = aG;
	color[2] = aB;
	color[3] = aA;
	aDeviceContext->ClearRenderTargetView(myRenderTargetData.RenderTargetView.Get(), color);
	aDeviceContext->ClearDepthStencilView(myRenderTargetData.DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ComPtr<ID3D11ShaderResourceView> FGE::RenderTexture::GetShaderResourceView()
{
	return myShaderResourceView;
}

ComPtr<ID3D11RenderTargetView> FGE::RenderTexture::GetRenderTargetView()
{
	return myRenderTargetData.RenderTargetView;
}

ComPtr<ID3D11DepthStencilView> FGE::RenderTexture::GetDepthStencilView()
{
	return myRenderTargetData.DepthStencilView;
}
