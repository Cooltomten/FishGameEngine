#pragma once
#include "FGE/Asset/Asset.h"

#include "FGE/Asset/EmitterSettingsData.h"
#include "FGE/Particles/ParticleVertex.h"

#include "FGE/Rendering/Buffers/VertexBuffer.h"

#include <CommonUtilities/Math/Matrix4x4.hpp>

#include <wrl.h>
#include <vector>
#include <memory>

namespace CU = CommonUtilities;

struct ID3D11VertexShader;
struct ID3D11GeometryShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;


namespace FGE
{
	class Texture;
	class ParticleEmitter
	{


		std::shared_ptr<EmitterSettingsData> myEmitterSettings;

		std::vector<ParticleVertex> myParticles;


		ComPtr<ID3D11Buffer> myVertexBuffer;
		UINT myStride;
		UINT myOffset;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> myVertexShader;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> myGeometryShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> myPixelShader;
		UINT myPrimitiveTopology;
		ComPtr<ID3D11InputLayout> myInputLayout;

		std::shared_ptr<Texture>myTexture;

		void InitParticle(ParticleVertex& aParticle);

		float myTimer;
		float myTimeBetweenParticles;

	public:
		ParticleEmitter() = default;
		~ParticleEmitter() = default;


		virtual bool Init(const std::shared_ptr<EmitterSettingsData>& aTemplate);
		void Update(float aTimeStep);

		virtual void SetAsResource() const;
		virtual void Render(const CU::Matrix4x4<float>& aTransform) const;

		//FORCEINLINE const EmitterSettingsData& GetEmitterSettings() const { return myEmitterSettings; } // unsure if i need this




	};

}