#include "GraphicsEngine.pch.h"
#include "Transform.h"
namespace FGE
{
	void Transform::SetPosition(float aX, float aY, float aZ)
	{
		SetPosition({ aX, aY, aZ });
	}

	void Transform::SetPosition(CU::Vector3f aPosition)
	{
		myPosition = aPosition;
		myMatrix(4, 1) = myPosition.x;
		myMatrix(4, 2) = myPosition.y;
		myMatrix(4, 3) = myPosition.z;
	}

	void Transform::SetRotation(float aX, float aY, float aZ)
	{
		SetRotation({ aX, aY, aZ });
	}

	void Transform::SetRotation(CU::Vector3f aRotation)
	{
		CU::Vector3f rotDiff = aRotation - myRotation;
		myRotation = aRotation;
		myMatrix = myMatrix.CreateRotationAroundX(rotDiff.x) * myMatrix;
		myMatrix = myMatrix.CreateRotationAroundY(rotDiff.y) * myMatrix;
		myMatrix = myMatrix.CreateRotationAroundZ(rotDiff.z) * myMatrix;
	}

	void Transform::SetScale(float aX, float aY, float aZ)
	{
		SetScale({ aX, aY, aZ });
	}

	void Transform::SetScale(CU::Vector3f aScale)
	{
		CommonUtilities::Vector3f scaleFactor;
		if (myScale.x != 0)
		{
			scaleFactor.x = aScale.x / myScale.x;
		}
		if (myScale.y != 0)
		{
			scaleFactor.y = aScale.y / myScale.y;
		}
		if (myScale.z != 0)
		{
			scaleFactor.z = aScale.z / myScale.z;
		}
		myScale = aScale;

		myMatrix(1, 1) *= scaleFactor.x;
		myMatrix(2, 2) *= scaleFactor.y;
		myMatrix(3, 3) *= scaleFactor.z;
	}

	const CU::Vector3f& Transform::GetPosition() const
	{
		return myPosition;
	}

	const CU::Vector3f& Transform::GetRotation() const
	{
		return myRotation;
	}

	const CU::Vector3f& Transform::GetScale() const
	{
		return myScale;
	}

	const CU::Matrix4x4<float>& Transform::GetMatrix() const 
	{
		return myMatrix;
	}

}