#include "GraphicsEngine.pch.h"
#include "Renderer.h"

#include "FGE/Core/Application.h"
#include "FGE/Asset/Material.h"
#include "FGE/Rendering/Buffers/VertexBuffer.h"
#include "FGE/Rendering/Lights/DirectionalLight.h"
#include "FGE/Rendering/Lights/EnvironmentLight.h"

#include <fstream>
namespace FGE
{
	Renderer::FrameBufferData Renderer::myFrameBufferData;
	Renderer::ObjectBufferData Renderer::myObjectBufferData;
	Renderer::MaterialBufferData Renderer::myMaterialBufferData;

	Microsoft::WRL::ComPtr<ID3D11Buffer> Renderer::myFrameBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> Renderer::myObjectBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> Renderer::myMaterialBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> Renderer::myLightBuffer;


	std::vector<SubmitCommand> Renderer::myCommands;
	std::shared_ptr<DirectionalLight> Renderer::myDirectionalLight;
	std::shared_ptr<EnvironmentLight> Renderer::myEnvironmentLight;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> Renderer::myVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> Renderer::myPixelShader;

	std::string Renderer::myVsData;

	RenderMode Renderer::myRenderMode = RenderMode::Default;


	static std::string myVsData;
	void Renderer::Init()
	{
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		//Create shaders
		std::ifstream vsFile;
		vsFile.open("Assets/Shaders/DefaultVS.cso", std::ios::binary);
		myVsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
		vsFile.close();
		ID3D11VertexShader* vertexShader = dx11.CreateVertexShader(myVsData.data(), myVsData.size());
		myVertexShader = vertexShader;;

		std::ifstream psFile;
		psFile.open("Assets/Shaders/DefaultPS.cso", std::ios::binary);
		std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
		psFile.close();
		ID3D11PixelShader* pixelShader = dx11.CreatePixelShader(psData.data(), psData.size());
		myPixelShader = pixelShader;
		//end shaders

		D3D11_BUFFER_DESC bufferDescription = { 0 };
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		bufferDescription.ByteWidth = sizeof(FrameBufferData);
		myFrameBuffer = dx11.CreateBuffer(&bufferDescription, nullptr);

		bufferDescription.ByteWidth = sizeof(ObjectBufferData);
		myObjectBuffer = dx11.CreateBuffer(&bufferDescription, nullptr);

		bufferDescription.ByteWidth = sizeof(MaterialBufferData);
		myMaterialBuffer = dx11.CreateBuffer(&bufferDescription, nullptr);

		bufferDescription.ByteWidth = sizeof(Light::LightBufferData);
		myLightBuffer = dx11.CreateBuffer(&bufferDescription, nullptr);
	}
	void FGE::Renderer::Submit(std::shared_ptr<VertexArray> aData, const CU::Matrix4x4<float>& aTransform,
		std::shared_ptr<Material> aMaterial, std::vector<CU::Matrix4x4<float>> someAnimData)
	{
		myCommands.emplace_back(SubmitCommand(aData, aTransform, aMaterial, someAnimData));
	}
	void Renderer::Begin(std::shared_ptr<Camera> aCamera)
	{
		myFrameBufferData.View = aCamera->GetViewMatrix();
		myFrameBufferData.Projection = aCamera->GetProjectionMatrix();
		myFrameBufferData.RenderMode = static_cast<uint32_t>(myRenderMode);
	}
	void Renderer::End()
	{
		myCommands.clear();
	}
	void Renderer::Render()
	{
		D3D11_MAPPED_SUBRESOURCE bufferData;

		if (myDirectionalLight)
		{
			myDirectionalLight->SetAsResource(myLightBuffer);
		}
		if (myEnvironmentLight)
		{
			myEnvironmentLight->SetAsResource(nullptr);
		}


		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		dx11.Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));
		dx11.Unmap(myFrameBuffer.Get(), 0);

		dx11.VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
		dx11.PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());


		auto& context = dx11.GetDeviceContext();

		context->VSSetShader(myVertexShader.Get(), nullptr, 0);
		context->PSSetShader(myPixelShader.Get(), nullptr, 0);

		for (auto& command : myCommands)
		{
			//map Object buffer
			myObjectBufferData.World = command.Transform;
			ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

			myObjectBufferData.HasBones = false;
			if (!command.AnimData.empty())
			{
				myObjectBufferData.HasBones = true;
				memcpy_s(&myObjectBufferData.BoneData[0], sizeof(CU::Matrix4x4<float>) * 128, &command.AnimData[0], sizeof(CU::Matrix4x4<float>) * 128);
			}
			dx11.Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
			memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
			dx11.Unmap(myObjectBuffer.Get(), 0);

			command.Material->SetAsResource(myMaterialBuffer);

			command.Data->Bind();

			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->IASetInputLayout(command.Data->GetInputLayout());


			context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
			context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
			context->PSSetConstantBuffers(2, 1, myMaterialBuffer.GetAddressOf());

			context->DrawIndexed(command.Data->GetIndexBuffer().GetCount(), 0, 0);
		}
	}
	void Renderer::SetDirectionalLight(std::shared_ptr<DirectionalLight> aLight)
	{
		myDirectionalLight = aLight;
	}
	void Renderer::SetEnvironmentLight(std::shared_ptr<EnvironmentLight> aLight)
	{
		myEnvironmentLight = aLight;
	}
	void Renderer::SetRenderMode(RenderMode aRenderMode)
	{
		myRenderMode = aRenderMode;
	}
	const RenderMode& Renderer::GetRenderMode()
	{
		return myRenderMode;
	}
}
