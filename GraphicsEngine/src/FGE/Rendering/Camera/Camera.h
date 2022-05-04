#pragma once
#include "CommonUtilities/Math/Vector2.hpp"
#include "FGE/Transform.h"
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
		Transform& GetTransform();


	private:
		Transform myTransform;
		CU::Matrix4x4<float> myProjectionMatrix;

		float myNearPlane;
		float myFarPlane;
		


	};
}