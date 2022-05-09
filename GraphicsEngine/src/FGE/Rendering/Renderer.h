#pragma once
#include "FGE/Rendering/Camera/Camera.h"
#include "FGE/Rendering/Buffers/VertexArray.h"
#include <memory>
#include <CommonUtilities/Math/Matrix4x4.hpp>
#include <vector>
#include <wrl.h>

namespace CU = CommonUtilities;

struct ID3D11VertexShader;
struct ID3D11PixelShader;

namespace FGE
{

	struct SubmitCommand
	{
		SubmitCommand(std::shared_ptr<VertexArray> aData, CU::Matrix4x4<float> aTransform)
			: Data(aData), Transform(aTransform) {}
		std::shared_ptr<VertexArray> Data;
		CU::Matrix4x4<float> Transform;
	};
	class Renderer
	{
	public:
		static void Init();

		static void Submit(std::shared_ptr<VertexArray> aData , const CU::Matrix4x4<float>& aTransform);

		static void Begin(std::shared_ptr<Camera> aCamera);
		static void End();
		static void Render();

		inline static Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShaderBlob() { return myVertexShader; }
		inline static Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShaderBlob() { return myPixelShader; }
		inline static std::string& GetVsData(){ return myVsData; }


	private:
		static struct FrameBufferData
		{
			CU::Matrix4x4<float> View;
			CU::Matrix4x4<float> Projection;
		}myFrameBufferData;
		static struct ObjectBufferData
		{
			CU::Matrix4x4<float> World;
		} myObjectBufferData;
		
		static Microsoft::WRL::ComPtr<ID3D11Buffer> myFrameBuffer;
		static Microsoft::WRL::ComPtr<ID3D11Buffer> myObjectBuffer;

		static std::vector<SubmitCommand> myCommands;
		static Microsoft::WRL::ComPtr<ID3D11VertexShader> myVertexShader;
		static Microsoft::WRL::ComPtr<ID3D11PixelShader> myPixelShader;

		static std::string myVsData;
	};
}