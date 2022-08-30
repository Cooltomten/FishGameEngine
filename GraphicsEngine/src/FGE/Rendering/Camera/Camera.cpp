#include "GraphicsEngine.pch.h"
#include "Camera.h"
#include <cassert>

#include "DirectXTK/SimpleMath.h"

namespace FGE
{
	void FGE::Camera::SetPerspectiveProjection(float aHorizontalFoV, CU::Vector2<unsigned int> aResolution, float aNearPlane, float aFarPlane)
	{
		assert(aNearPlane < aFarPlane);
		assert(aNearPlane >= 0.0001f);

		myNearPlane = aNearPlane;
		myFarPlane = aFarPlane;

		const float hFoVRad = aHorizontalFoV * (PIF / 180.f);

		const float vFovRad = 2.f * atan(tan(hFoVRad / 2.f) * (aResolution.y / (float)aResolution.x));

		const float myXScale = 1.f / tan(hFoVRad / 2.f);
		const float myYScale = 1.f / tan(vFovRad / 2.f);
		const float Q = myFarPlane / (myFarPlane - myNearPlane);

		myProjectionMatrix(1, 1) = myXScale;
		myProjectionMatrix(2, 2) = myYScale;
		myProjectionMatrix(3, 3) = Q;
		myProjectionMatrix(3, 4) = 1.0f / Q;
		myProjectionMatrix(4, 3) = -Q * myNearPlane;
		myProjectionMatrix(4, 4) = 0.f;

		return;
	}
	const CU::Matrix4x4<float>& Camera::GetProjectionMatrix() const
	{
		return myProjectionMatrix;
	}
	const CU::Matrix4x4<float>& Camera::GetViewMatrix() const
	{
		return myViewMatrix;
	}
	const CU::Matrix4x4<float>& Camera::GetTransformMatrix() const
	{
		return myTransformMatrix;
	}
	void Camera::RecalculateViewMatrix(CU::Matrix4x4<float> aTransform)
	{
		myViewMatrix = aTransform.GetFastInverse(aTransform);
		myTransformMatrix = aTransform;
	}
}
