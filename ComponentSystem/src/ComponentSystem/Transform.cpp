#include "CompSysPch.h"
#include "Transform.h"
namespace Comp
{
	Transform::Transform()
	{
		myPosition = { 0,0,0 };
		myRotation = { 0,0,0 };
		myScale = { 1,1,1 };
		RecalculateTransform();
	}
	
	void Transform::SetPosition(float aX, float aY, float aZ)
	{
		SetPosition({ aX, aY, aZ });
	}

	void Transform::SetPosition(CU::Vector3f aPosition)
	{
		myPosition = aPosition;
		RecalculateTransform();
	}

	void Transform::SetRotation(float aX, float aY, float aZ)
	{
		SetRotation({ aX, aY, aZ });
	}

	void Transform::SetRotation(CU::Vector3f aRotation)
	{
		myRotation = aRotation;
		RecalculateTransform();

	}

	void Transform::SetScale(float aX, float aY, float aZ)
	{
		SetScale({ aX, aY, aZ });
	}

	void Transform::SetScale(CU::Vector3f aScale)
	{
		myScale = aScale;
		RecalculateTransform();

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

	const CU::Matrix4x4<float>& Transform::GetInverseMatrix() const
	{
		return myInverseMatrix;
	}

	const CU::Vector3f Transform::GetForward()
	{
		return {myMatrix(3,1), myMatrix(3,2), myMatrix(3,3)};
	}

	const CU::Vector3f Transform::GetRight()
	{
		return {myMatrix(1,1), myMatrix(1,2), myMatrix(1,3)};
	}

	const CU::Vector3f Transform::GetUp()
	{
		return {myMatrix(2,1), myMatrix(2,2), myMatrix(2,3)};
	}

	void Transform::RecalculateTransform()
	{
		myMatrix = CU::Matrix4x4<float>();
		
		myMatrix(1, 1) = myScale.x;
		myMatrix(2, 2) = myScale.y;
		myMatrix(3, 3) = myScale.z;
		
		CommonUtilities::Vector3f rotInRad = myRotation * 3.14159265359f / 180.0f;
		auto rotX =  CU::Matrix4x4<float>::CreateRotationAroundX(rotInRad.x);
		auto rotY =  CU::Matrix4x4<float>::CreateRotationAroundY(rotInRad.y);
		auto rotZ =  CU::Matrix4x4<float>::CreateRotationAroundZ(rotInRad.z);

		auto rot = rotX* rotY * rotZ;
		
		myMatrix = myMatrix* rot ;
		myMatrix(4, 1) = myPosition.x;
		myMatrix(4, 2) = myPosition.y;
		myMatrix(4, 3) = myPosition.z;

		myInverseMatrix = myMatrix.GetFastInverse(myMatrix);
	}

}