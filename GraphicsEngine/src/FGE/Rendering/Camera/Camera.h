#pragma once
#include "CommonUtilities/Math/Vector2.hpp"
#include "CommonUtilities/Math/Matrix4x4.hpp"
namespace CU = CommonUtilities;

namespace FGE
{
	class Camera
	{
	public:
		Camera() = default;
		~Camera() = default;
		void SetPerspectiveProjection(float aHorizontalFoV, CU::Vector2<unsigned int> aResolution, float aNearPlane, float aFarPlane);

		const CU::Matrix4x4<float>& GetProjectionMatrix() const;
		const CU::Matrix4x4<float>& GetViewMatrix() const;
		const CU::Matrix4x4<float>& GetTransformMatrix() const;

		void RecalculateViewMatrix(CU::Matrix4x4<float> aTransform);

	private:
		CU::Matrix4x4<float> myProjectionMatrix;
		CU::Matrix4x4<float> myViewMatrix;
		CU::Matrix4x4<float> myTransformMatrix;

		float myNearPlane;
		float myFarPlane;
		


	};
}