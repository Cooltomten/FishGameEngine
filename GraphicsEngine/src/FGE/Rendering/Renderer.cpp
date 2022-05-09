#include "GraphicsEngine.pch.h"
#include "Renderer.h"
#include "FGE/Core/Application.h"
#include "FGE/Rendering/Buffers/VertexBuffer.h"
#include <fstream>
namespace FGE
{
	Renderer::FrameBufferData Renderer::myFrameBufferData;
	Renderer::ObjectBufferData Renderer::myObjectBufferData;

	 Microsoft::WRL::ComPtr<ID3D11Buffer> Renderer::myFrameBuffer;
	 Microsoft::WRL::ComPtr<ID3D11Buffer> Renderer::myObjectBuffer;

	 std::vector<SubmitCommand> Renderer::myCommands;
	 Microsoft::WRL::ComPtr<ID3D11VertexShader> Renderer::myVertexShader;
	 Microsoft::WRL::ComPtr<ID3D11PixelShader> Renderer::myPixelShader;

	  std::string Renderer::myVsData;
	

	static std::string myVsData;
	void Renderer::Init()
	{
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		//Create shaders
		std::ifstream vsFile;
		vsFile.open("DefaultVS.cso", std::ios::binary);
		myVsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
		vsFile.close();
		ID3D11VertexShader* vertexShader = dx11.CreateVertexShader(myVsData.data(), myVsData.size());
		myVertexShader = vertexShader;;

		std::ifstream psFile;
		psFile.open("DefaultPS.cso", std::ios::binary);
		std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
		psFile.close();
		ID3D11PixelShader* pixelShader = dx11.CreatePixelShader(psData.data(), psData.size());
		myPixelShader = pixelShader;
		//end shader

		D3D11_BUFFER_DESC bufferDescription = { 0 };
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		bufferDescription.ByteWidth = sizeof(FrameBufferData);
		myFrameBuffer = dx11.CreateBuffer(&bufferDescription, nullptr);

		bufferDescription.ByteWidth = sizeof(ObjectBufferData);
		myObjectBuffer = dx11.CreateBuffer(&bufferDescription, nullptr);
	}
	void FGE::Renderer::Submit(std::shared_ptr<VertexArray> aData, const CU::Matrix4x4<float>& aTransform)
	{
		myCommands.emplace_back(SubmitCommand(aData, aTransform));
	}
	void Renderer::Begin(std::shared_ptr<Camera> aCamera)
	{
		myFrameBufferData.View = aCamera->GetViewMatrix();
		myFrameBufferData.Projection = aCamera->GetProjectionMatrix();
	}
	void Renderer::End()
	{
		myCommands.clear();
	}
	void Renderer::Render()
	{
		D3D11_MAPPED_SUBRESOURCE bufferData;

		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		dx11.Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));
		dx11.Unmap(myFrameBuffer.Get(), 0);

		dx11.VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());


		auto& context = dx11.GetDeviceContext();
		
		context->VSSetShader(myVertexShader.Get(), nullptr, 0);
		context->PSSetShader(myPixelShader.Get(), nullptr, 0);

		for (auto& command : myCommands)
		{
			myObjectBufferData.World = command.Transform;

			ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
			dx11.Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
			memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
			dx11.Unmap(myObjectBuffer.Get(), 0);


			command.Data->Bind();

			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->IASetInputLayout(command.Data->GetInputLayout());


			context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
			context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

			context->DrawIndexed(command.Data->GetIndexBuffer().GetCount(), 0, 0);
		}
	}
}