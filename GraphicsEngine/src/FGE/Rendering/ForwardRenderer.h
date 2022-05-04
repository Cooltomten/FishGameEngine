#pragma once
#include <wrl.h>a
#include <CommonUtilities/Math/Matrix4x4.hpp>
#include <vector>
namespace CU = CommonUtilities;
struct ID3D11Buffer;
namespace FGE
{
	class Camera;
	class Model;
	class ForwardRenderer
	{

	public:
		void Initialize();

		void Render(const std::shared_ptr<Camera>& aCamera, const std::vector<std::shared_ptr<Model>>& aModelList );
	private:
		struct FrameBufferData
		{
			CU::Matrix4x4<float> View;
			CU::Matrix4x4<float> Projection;
		}myFrameBufferData;

		struct ObjectBufferData
		{
			CU::Matrix4x4<float> World;
		} myObjectBufferData;

		Microsoft::WRL::ComPtr<ID3D11Buffer> myFrameBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> myObjectBuffer;
	};
}