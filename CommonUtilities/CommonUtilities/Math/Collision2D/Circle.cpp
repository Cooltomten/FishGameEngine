#include "Math/Collision2D/Circle.h"

namespace CommonUtilities
{
	Circle::Circle()
	{
		myShape = Shape2D::Circle;
		myRadius = 0;
		myPosition = { 0,0 };
	}
	Circle::Circle(Vector2f aPosition, float aRadius)
	{
		myShape = Shape2D::Circle;
		myPosition = aPosition;
		myRadius = aRadius;
	}
	Circle::~Circle()
	{
	}
	bool Circle::IsInside(const Vector2f& aPosition) const
	{
		if ((aPosition - myPosition).LengthSqr() < myRadius * myRadius) 
		{
			return true;
		}
		return false;
	}

	float& Circle::Radius()
	{
		return myRadius;
	}
	float Circle::Radius() const
	{
		return myRadius;
	}
}
