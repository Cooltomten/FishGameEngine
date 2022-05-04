#pragma once
#include "Collider2D.h"
namespace CommonUtilities 
{
	class Rectangle : public Collider2D
	{
	public:
		Rectangle() = default;
		Rectangle(const Vector2f& aPosition, const Vector2f& aSize, float aRotation = 0);
		~Rectangle() ;
		
		bool IsInside(const Vector2f& aPosition) const override;

		Vector2f& Size();
		const Vector2f Size() const;

		float& Rotation();
		const float& Rotation() const;

	private:
		Vector2f mySize;
		float myRotation;
	};
}
