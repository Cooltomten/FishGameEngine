#include "GraphicsEngine.pch.h"
#include "DirectionalLight.h"
#include "FGE/Core/Application.h"
namespace FGE
{
	void DirectionalLight::SetAsResource(ComPtr<ID3D11Buffer> aLightBuffer)
	{
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		

		
		D3D11_MAPPED_SUBRESOURCE bufferData;
		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		
		dx11.Map(aLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		memcpy_s(bufferData.pData, sizeof(LightBufferData), &myLightBufferData, sizeof(LightBufferData));
		dx11.Unmap(aLightBuffer.Get(), 0);
		
		
		dx11.PSSetConstantBuffers(3, 1, aLightBuffer.GetAddressOf());
	}
	void DirectionalLight::SetDirection(CU::Vector3f aDirection)
	{
		myLightBufferData.Direction = aDirection;
	}
}
