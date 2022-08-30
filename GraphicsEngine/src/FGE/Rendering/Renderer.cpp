#include "GraphicsEngine.pch.h"
#include "Renderer.h"

#include "FGE/Core/Application.h"
#include "FGE/Asset/Material.h"
#include "FGE/Rendering/Buffers/VertexBuffer.h"
#include "FGE/Rendering/Lights/DirectionalLight.h"
#include "FGE/Rendering/Lights/EnvironmentLight.h"
#include "FGE/Particles/ParticleEmitter.h"

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


	std::vector<ModelCommand> Renderer::myModelCommands;
	std::shared_ptr<DirectionalLight> Renderer::myDirectionalLight;
	std::shared_ptr<EnvironmentLight> Renderer::myEnvironmentLight;

	std::vector<ParticleCommand> Renderer::myParticleCommands;


	Microsoft::WRL::ComPtr<ID3D11VertexShader> Renderer::myVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> Renderer::myPixelShader;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> Renderer::myFullscreenShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> Renderer::myEnvironmentShader;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> Renderer::myGBufferPixelShader;
	std::string Renderer::myVsData;

	RenderMode Renderer::myRenderMode = RenderMode::Default;

	std::array < Microsoft::WRL::ComPtr<ID3D11BlendState>, static_cast<unsigned int>(BlendState::COUNT) > Renderer::myBlendStates;
	std::array < Microsoft::WRL::ComPtr<ID3D11DepthStencilState>, static_cast<unsigned int>(DepthStencilState::COUNT) > Renderer::myDepthStencilStates;

	GBuffer Renderer::myGBuffer;





	static std::string myVsData;
	void Renderer::Init()
	{
		myGBuffer.Init();
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		//Create shaders
		std::ifstream vsFile;
		vsFile.open("Assets/Shaders/DefaultVS.cso", std::ios::binary);
		myVsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
		vsFile.close();
		ID3D11VertexShader* vertexShader = dx11.CreateVertexShader(myVsData.data(), myVsData.size());
		myVertexShader = vertexShader;

		std::ifstream psFile;
		psFile.open("Assets/Shaders/DefaultPS.cso", std::ios::binary);
		std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
		psFile.close();
		ID3D11PixelShader* pixelShader = dx11.CreatePixelShader(psData.data(), psData.size());
		myPixelShader = pixelShader;

		//Gbuffer pixel shader
		std::ifstream gbufferPsFile;
		gbufferPsFile.open("Assets/Shaders/GBufferPS.cso", std::ios::binary);
		std::string gbufferPsData = { std::istreambuf_iterator<char>(gbufferPsFile), std::istreambuf_iterator<char>() };
		gbufferPsFile.close();
		ID3D11PixelShader* gbufferPixelShader = dx11.CreatePixelShader(gbufferPsData.data(), gbufferPsData.size());
		myGBufferPixelShader = gbufferPixelShader;

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

		HRESULT hr;
		//alpha blend
		D3D11_BLEND_DESC alphaBlendDesc;
		alphaBlendDesc.AlphaToCoverageEnable = 0;
		alphaBlendDesc.IndependentBlendEnable = 0;
		alphaBlendDesc.RenderTarget[0].BlendEnable = true;
		alphaBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		alphaBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		alphaBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		alphaBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		alphaBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		alphaBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		alphaBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = dx11.GetDevice()->CreateBlendState(&alphaBlendDesc, myBlendStates[static_cast<unsigned>(BlendState::Alpha)].GetAddressOf());
		if (FAILED(hr))
		{
			throw;
		}
		//additive
		D3D11_BLEND_DESC additiveBlendDesc;
		additiveBlendDesc.AlphaToCoverageEnable = 0;
		additiveBlendDesc.IndependentBlendEnable = 0;
		additiveBlendDesc.RenderTarget[0].BlendEnable = true;
		additiveBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		additiveBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		additiveBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		additiveBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		additiveBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		additiveBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		additiveBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


		hr = dx11.GetDevice()->CreateBlendState(&additiveBlendDesc, myBlendStates[static_cast<unsigned>(BlendState::Additive)].GetAddressOf());

		if (FAILED(hr))
		{
			throw;
		}

		//default
		myBlendStates[static_cast<unsigned>(BlendState::None)] = nullptr;

		//Depth stencil states
		D3D11_DEPTH_STENCIL_DESC readOnlyDepthDesc = {};
		readOnlyDepthDesc.DepthEnable = true;
		readOnlyDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		readOnlyDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;
		readOnlyDepthDesc.StencilEnable = false;

		hr = dx11.GetDevice()->CreateDepthStencilState(&readOnlyDepthDesc, myDepthStencilStates[static_cast<unsigned>(DepthStencilState::ReadOnly)].GetAddressOf());

		if (FAILED(hr))
		{
			throw;
		}

		myDepthStencilStates[static_cast<unsigned>(DepthStencilState::ReadOnly)] = nullptr;






	}
	void FGE::Renderer::SubmitModel(std::shared_ptr<VertexArray> aData, const CU::Matrix4x4<float>& aTransform,
		std::shared_ptr<Material> aMaterial, std::vector<CU::Matrix4x4<float>> someAnimData)
	{
		myModelCommands.emplace_back(ModelCommand(aData, aTransform, aMaterial, someAnimData));
	}
	void Renderer::SubmitParticle(const ParticleEmitter* const aEmitter, const std::vector<ParticleVertex>& someVertices, const CU::Matrix4x4<float>& aTransform)
	{
		myParticleCommands.emplace_back(ParticleCommand(aEmitter, someVertices, aTransform));
	}
	void Renderer::Begin(std::shared_ptr<Camera> aCamera)
	{
		myGBuffer.Clear();
		myFrameBufferData.View = aCamera->GetViewMatrix();
		myFrameBufferData.Projection = aCamera->GetProjectionMatrix();
		myFrameBufferData.RenderMode = static_cast<uint32_t>(myRenderMode);
		auto& cameraTransform = aCamera->GetTransformMatrix();
		myFrameBufferData.CameraTranslation = { cameraTransform(4,1), cameraTransform(4,2), cameraTransform(4,3) };
	}
	void Renderer::End()
	{
		myModelCommands.clear();
		myParticleCommands.clear();
	}

	void Renderer::GenerateGBuffer()
	{
		D3D11_MAPPED_SUBRESOURCE bufferData;
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		auto& context = dx11.GetDeviceContext();

		//generate GBuffer
		myGBuffer.SetAsTarget(dx11.GetDepthStencilView().Get());
		context->VSSetShader(myVertexShader.Get(), nullptr, 0);
		context->PSSetShader(myGBufferPixelShader.Get(), nullptr, 0);

		for (auto& command : myModelCommands)
		{
			myObjectBufferData.World = command.Transform;
			myObjectBufferData.HasBones = false;

			//map Object buffer
			ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
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
		dx11.SetRenderTarget();

		context->VSSetShader(0, nullptr, 0);
		context->PSSetShader(0, nullptr, 0);
	}
	void Renderer::RenderGBuffer()
	{
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		auto& context = dx11.GetDeviceContext();

		//render Gbuffer
		if (myDirectionalLight)
		{
			myDirectionalLight->SetAsResource(myLightBuffer);
		}
		if (myEnvironmentLight)
		{
			myEnvironmentLight->SetAsResource(nullptr);
		}

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->IASetInputLayout(nullptr);
		context->IASetVertexBuffers(0,0,nullptr, nullptr, nullptr);
		
		context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

		context->GSSetShader(0, nullptr, 0);

		context->VSSetShader(myFullscreenShader.Get(), nullptr, 0);
		context->PSSetShader(myEnvironmentShader.Get(), nullptr, 0);

		context->Draw(3, 0);

	}
	void Renderer::RenderParticles()
	{
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		auto& context = dx11.GetDeviceContext();
		
		D3D11_MAPPED_SUBRESOURCE bufferData;
		
		SetBlendState(BlendState::Alpha);
		SetDepthStencilState(DepthStencilState::ReadOnly);
		for (auto& command : myParticleCommands)
		{
			//map Object buffer
			myObjectBufferData.World = command.Transform;
			ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

			myObjectBufferData.HasBones = false;

			dx11.Map(myObjectBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
			memcpy(bufferData.pData, &myObjectBufferData, sizeof(ObjectBufferData));
			dx11.Unmap(myObjectBuffer.Get(), 0);

			context->VSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
			context->GSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());
			context->PSSetConstantBuffers(1, 1, myObjectBuffer.GetAddressOf());

			command.Emitter->SetAsResource(); // changes vertex, geometry and pixel shader
			context->Draw(command.Vertices.size(), 0);
		}

		SetBlendState(BlendState::None);
		SetDepthStencilState(DepthStencilState::ReadWrite);

		context->VSSetShader(0, nullptr, 0);
		context->GSSetShader(0, nullptr, 0);
		context->PSSetShader(0, nullptr, 0);
	}

	void Renderer::Render()
	{
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		auto& context = dx11.GetDeviceContext();
		
		D3D11_MAPPED_SUBRESOURCE bufferData;
		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		dx11.Map(myFrameBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		memcpy(bufferData.pData, &myFrameBufferData, sizeof(FrameBufferData));
		dx11.Unmap(myFrameBuffer.Get(), 0);
		
		context->VSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
		context->GSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());
		context->PSSetConstantBuffers(0, 1, myFrameBuffer.GetAddressOf());

		GenerateGBuffer();

		//RenderGBuffer();

		RenderParticles();

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
	void Renderer::SetBlendState(BlendState aBlendState)
	{
		//dx11
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		dx11.GetDeviceContext()->OMSetBlendState(myBlendStates[static_cast<uint32_t>(aBlendState)].Get(), nullptr, 0xffffffff);
	}
	void Renderer::SetDepthStencilState(DepthStencilState aDepthStencilState)
	{
		//dx11
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		dx11.GetDeviceContext()->OMSetDepthStencilState(myDepthStencilStates[static_cast<uint32_t>(aDepthStencilState)].Get(), 0);
	}

}
