#pragma once
#include "Collider2D.h"
namespace CommonUtilities
{
	class Circle : public Collider2D
	{
	public:
		Circle();
		Circle(Vector2f aPosition, float aRadius);
		~Circle();

		bool IsInside(const Vector2f& aPosition) const override;

		float& Radius();
		float Radius() const;
	private:
		float myRadius;
	};
}