#include "GraphicsEngine.pch.h"
#include "ForwardRenderer.h"
#include "FGE/Core/DX11.h"
#include "FGE/Core/Window.h"
#include "FGE/Core/Application.h"
#include "FGE/Rendering/Camera/Camera.h"
#include "FGE/Asset/Model.h"

namespace FGE
{
	void ForwardRenderer::Initialize()
	{
		D3D11_BUFFER_DESC bufferDescription = { 0 };
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		bufferDescription.ByteWidth = sizeof(FrameBufferData);
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		myFrameBuffer = dx11.CreateBuffer(&bufferDescription, nullptr);

		bufferDescription.ByteWidth = sizeof(ObjectBufferData);
		myObjectBuffer = dx11.CreateBuffer(&bufferDescription, nullptr);
	}

	void ForwardRenderer::Render(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<Mesh>>& aModelList)
	{
		/*D3D11_MAPPED_SUBRESOURCE bufferData;

		myFrameBufferData.View = aCamera->GetViewMatrix();
		myFrameBufferData.Projection = aCamera->GetProjectionMatrix();

		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		dx11.Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));
		dx11.Unmap(myFrameBuffer.Get(), 0);
		
		dx11.VSSetConstantBuffers(0,1, myFrameBuffer.GetAddressOf());

		for (auto& model : aModelList)
		{
			const Mesh::MeshData& meshData = model->GetMeshData();
			myObjectBufferData.World = model->GetTransform().GetMatrix();

			ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
			dx11.Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
			memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
			dx11.Unmap(myObjectBuffer.Get(), 0);
			
			auto& context = dx11.GetDeviceContext();
			
			context->IASetVertexBuffers(0, 1, meshData.VertexBuffer.GetAddressOf(), &meshData.Stride, &meshData.Offset);
			context->IASetIndexBuffer(meshData.IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			
			context->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(meshData.PrimitiveTopology));
			context->IASetInputLayout(meshData.InputLayout.Get());

			context->VSSetShader(meshData.VertexShader.Get(), nullptr, 0);
			context->PSSetShader(meshData.PixelShader.Get(), nullptr, 0);
			
			context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
			context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
			
			context->DrawIndexed(meshData.NumberOfIndices, 0, 0);

		}*/
	}

}