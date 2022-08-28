#include "GraphicsEngine.pch.h"
#include "ParticleEmitter.h"
#include "FGE/Core/Application.h"
#include "FGE/Asset/ResourceCache.h"
#include "FGE/Asset/Texture.h"
#include "FGE/Rendering/Renderer.h"

namespace FGE
{

	void FGE::ParticleEmitter::InitParticle(ParticleVertex& aParticle)
	{
		aParticle.Color = myEmitterSettings->StartColor;
		aParticle.Position = { 0,0,0,1 };
		aParticle.Velocity = myEmitterSettings->StartVelocity;
		aParticle.Scale = { myEmitterSettings->StartScale,myEmitterSettings->StartScale,myEmitterSettings->StartScale };
		aParticle.LifeTime = myEmitterSettings->LifeTime;

	}

	bool FGE::ParticleEmitter::Init(const std::shared_ptr<EmitterSettingsData>& aTemplate)
	{
		myEmitterSettings = aTemplate;

		myTimeBetweenParticles = 1 / myEmitterSettings->ParticlesPerSecond;

		myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;

		const size_t maxNumberOfParticles = static_cast<size_t>(ceilf(myEmitterSettings->ParticlesPerSecond * myEmitterSettings->LifeTime));

		myParticles.resize(maxNumberOfParticles);

		auto& dx11 = Application::Get().GetWindow()->GetDX11();

		myStride = sizeof(ParticleVertex);

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(ParticleVertex) * maxNumberOfParticles;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		myVertexBuffer = dx11.CreateBuffer(&bufferDesc, nullptr);


		//VertexShader
		std::ifstream vsFile;
		vsFile.open("Assets/Shaders/ParticleVS.cso", std::ios::binary);
		std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
		vsFile.close();
		ID3D11VertexShader* vertexShader = dx11.CreateVertexShader(vsData.data(), vsData.size());
		myVertexShader = vertexShader;

		//GeometryShader 
		std::ifstream gsFile;
		gsFile.open("Assets/Shaders/ParticleGS.cso", std::ios::binary);
		std::string gsData = { std::istreambuf_iterator<char>(gsFile), std::istreambuf_iterator<char>() };
		gsFile.close();
		ID3D11GeometryShader* geometryShader = dx11.CreateGeometryShader(gsData.data(), gsData.size());
		myGeometryShader = geometryShader;

		//PixelShader
		std::ifstream psFile;
		psFile.open("Assets/Shaders/ParticlePS.cso", std::ios::binary);
		std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
		psFile.close();
		ID3D11PixelShader* pixelShader = dx11.CreatePixelShader(psData.data(), psData.size());
		myPixelShader = pixelShader;

		//InputLayout
		D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SCALE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "LIFETIME", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		myInputLayout = dx11.CreateInputLayout(inputLayoutDesc, ARRAYSIZE(inputLayoutDesc), vsData.data(), vsData.size());

		myTexture = ResourceCache::GetAsset<Texture>("Assets/Textures/ParticleStar.dds");


		return true;
	}

	void FGE::ParticleEmitter::Update(float aTimeStep)
	{
		myTimer += aTimeStep;
		static int particlesToCreate = 0;
		while (myTimer >= myTimeBetweenParticles)
		{
			++particlesToCreate;
			myTimer -= myTimeBetweenParticles;
		}
		for (int i = 0; i < myParticles.size(); i++)
		{
			ParticleVertex& particle = myParticles[i];
			particle.LifeTime -= aTimeStep;
			if (particle.LifeTime <= 0.0f)
			{
				if (particlesToCreate > 0)
				{
					InitParticle(particle);
					--particlesToCreate;
				}
				else
				{
					particle.Color.w = 0;
					continue;
				}
			}
			particle.Position += CU::Vector4f(particle.Velocity.x, particle.Velocity.y, particle.Velocity.z) * aTimeStep;

		}
	}

	void ParticleEmitter::SetAsResource() const
	{
		auto& dx11 = Application::Get().GetWindow()->GetDX11();
		auto& context = dx11.GetDeviceContext();

		D3D11_MAPPED_SUBRESOURCE bufferData;
		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		dx11.Map(myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);

		memcpy_s(bufferData.pData,
			sizeof(ParticleVertex) * myParticles.size(),
			&myParticles[0],
			sizeof(ParticleVertex) * myParticles.size());

		dx11.Unmap(myVertexBuffer.Get(), 0);

		context->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(myPrimitiveTopology));
		context->IASetInputLayout(myInputLayout.Get());
		context->IASetVertexBuffers(0, 1, myVertexBuffer.GetAddressOf(), &myStride, &myOffset);

		context->VSSetShader(myVertexShader.Get(), nullptr, 0);
		context->GSSetShader(myGeometryShader.Get(), nullptr, 0);
		context->PSSetShader(myPixelShader.Get(), nullptr, 0);

		myTexture->SetAsResource(0);

	}

	void ParticleEmitter::Render(const CU::Matrix4x4<float>& aTransform) const
	{
		Renderer::SubmitParticle(this, myParticles, aTransform);
	}

}