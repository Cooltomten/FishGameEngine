#pragma once
#include "CommonUtilities/Math/Vector3.hpp"
#include "CommonUtilities/Math/Matrix4x4.hpp"

namespace CU = CommonUtilities;
namespace FGE
{
	class Transform
	{
	public:
		Transform();
		~Transform() = default;

		void SetPosition(float aX, float aY, float aZ);
		void SetPosition(CU::Vector3f aPosition);

		void SetRotation(float aX, float aY, float aZ);
		void SetRotation(CU::Vector3f aRotation);

		void SetScale(float aX, float aY, float aZ);
		void SetScale(CU::Vector3f aScale);

		const CU::Vector3f& GetPosition() const;
		const CU::Vector3f& GetRotation() const;
		const CU::Vector3f& GetScale() const;

		const CU::Matrix4x4<float>& GetMatrix() const;
		const CU::Matrix4x4<float>& GetInverseMatrix() const;

		const CU::Vector3f GetForward();
		const CU::Vector3f GetRight();
		const CU::Vector3f GetUp();
		
	private:
		void RecalculateTransform();
		CU::Vector3f myPosition;
		CU::Vector3f myRotation;
		CU::Vector3f myScale;

		CU::Matrix4x4<float> myMatrix;
		CU::Matrix4x4<float> myInverseMatrix;
		


	};
}
