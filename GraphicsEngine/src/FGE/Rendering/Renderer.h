#pragma once
#include "FGE/Rendering/Camera/Camera.h"
#include "FGE/Rendering/Buffers/VertexArray.h"
#include "FGE/Rendering/Buffers/GBuffer.h"

#include "FGE/Particles/ParticleVertex.h"
#include "FGE/Rendering/RenderTargetData.h"


#include <CommonUtilities/Math/Vector.hpp>
#include <CommonUtilities/Math/Matrix4x4.hpp>


#include <memory>
#include <vector>
#include <array>
#include <wrl.h>

namespace CU = CommonUtilities;

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11DepthStencilState;

namespace FGE
{
	enum class RenderMode
	{
		Default,
		UV1,
		UV2,
		UV3,
		UV4,
		VertexColors1,
		VertexColors2,
		VertexColors3,
		VertexColors4,
		VertexNormal,
		PixelNormal,
		AlbedoMap,
		AmbientOcclusion,
		Roughness,
		Metalness,
		Emissiveness,
		NormalMap,
		DiffuseLight,
		AmbientLight,
		DiffuseNoAlbedo,
		AmbientNoAlbedo,
		COUNT


	};

	enum class BlendState : unsigned int
	{
		None,
		Alpha,
		Additive,
		COUNT
	};

	enum class DepthStencilState : unsigned int
	{
		ReadWrite,
		ReadOnly, 
		COUNT
	};
	class Material;
	class DirectionalLight;
	class EnvironmentLight;
	class ParticleEmitter;
	struct ModelCommand
	{
		ModelCommand(std::shared_ptr<VertexArray> aData, CU::Matrix4x4<float> aTransform, std::shared_ptr<Material> aMaterial, std::vector<CU::Matrix4x4<float>> someAnimData)
			: Data(aData), Transform(aTransform), Material(aMaterial), AnimData(someAnimData) {}

		std::shared_ptr<VertexArray> Data;
		std::shared_ptr<Material> Material;
		CU::Matrix4x4<float> Transform;
		std::vector<CU::Matrix4x4<float>> AnimData;
	};
	struct ParticleCommand
	{
		ParticleCommand(const ParticleEmitter* const aEmitter, const std::vector<ParticleVertex>& someVertices, CU::Matrix4x4<float> aTransform)
			: Emitter(aEmitter), Vertices(someVertices), Transform(aTransform) {}

		const ParticleEmitter* const Emitter;
		const std::vector<ParticleVertex>& Vertices;
		CU::Matrix4x4<float> Transform;
	};
	class Renderer
	{
	public:
		static void Init();

		static void SubmitModel(std::shared_ptr<VertexArray> aData, const CU::Matrix4x4<float>& aTransform,
			std::shared_ptr<Material> aMaterial, std::vector<CU::Matrix4x4<float>> someAnimData = {});

		static void SubmitParticle(const ParticleEmitter* const aEmitter, const std::vector<ParticleVertex>& someVertices, const CU::Matrix4x4<float>& aTransform);

		static void Begin(std::shared_ptr<Camera> aCamera);
		static void End();
		static void Render();

		inline static Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShaderBlob() { return myVertexShader; }
		inline static Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShaderBlob() { return myPixelShader; }
		inline static std::string& GetVsData() { return myVsData; }

		static void SetDirectionalLight(std::shared_ptr<DirectionalLight> aLight);
		static void SetEnvironmentLight(std::shared_ptr<EnvironmentLight> aLight);

		static void SetRenderMode(RenderMode aRenderMode);
		static const RenderMode& GetRenderMode();

		static void SetBlendState(BlendState aBlendState);
		static void SetDepthStencilState(DepthStencilState aDepthStencilState);

		static void SetRenderTarget(const RenderTargetData& someRenderTargetData);


	private:
		static void GenerateGBuffer();
		static void RenderGBuffer();
		static void RenderParticles();

		static struct FrameBufferData
		{
			CU::Matrix4x4<float> View;
			CU::Matrix4x4<float> Projection;
			CU::Vector3f CameraTranslation;
			uint32_t RenderMode;
		}myFrameBufferData;

		static struct ObjectBufferData
		{
			CU::Matrix4x4<float> World;
			CU::Matrix4x4<float> BoneData[128];
			alignas(16) bool HasBones;
		} myObjectBufferData;

		static struct MaterialBufferData
		{
			CU::Vector3f Albedo;
			float Padding;
		} myMaterialBufferData;



		static Microsoft::WRL::ComPtr<ID3D11Buffer> myFrameBuffer;
		static Microsoft::WRL::ComPtr<ID3D11Buffer> myObjectBuffer;
		static Microsoft::WRL::ComPtr<ID3D11Buffer> myMaterialBuffer;
		static Microsoft::WRL::ComPtr<ID3D11Buffer> myLightBuffer;

		static std::vector<ModelCommand> myModelCommands;
		static std::shared_ptr<DirectionalLight> myDirectionalLight;
		static std::shared_ptr<EnvironmentLight> myEnvironmentLight;

		static std::vector<ParticleCommand> myParticleCommands;


		static Microsoft::WRL::ComPtr<ID3D11VertexShader> myVertexShader;
		static Microsoft::WRL::ComPtr<ID3D11PixelShader> myPixelShader;
		
		static Microsoft::WRL::ComPtr<ID3D11PixelShader> myGBufferPixelShader;
		
		static Microsoft::WRL::ComPtr<ID3D11VertexShader> myFullscreenShader;
		static Microsoft::WRL::ComPtr<ID3D11PixelShader> myEnvironmentShader;		

		static std::string myVsData;

		static RenderMode myRenderMode;
		static std::array < Microsoft::WRL::ComPtr<ID3D11BlendState>, static_cast<unsigned int>(BlendState::COUNT) > myBlendStates;
		static std::array < Microsoft::WRL::ComPtr<ID3D11DepthStencilState>, static_cast<unsigned int>(DepthStencilState::COUNT) > myDepthStencilStates;

		static GBuffer myGBuffer;
		
		static const RenderTargetData* myCurrentRenderTargetData;

	};
}