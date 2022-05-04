#include "Math/Collision2D/Collider2D.h"


namespace CommonUtilities
{

	Vector2f& Collider2D::Position()
	{
		return myPosition;
	}
	const Vector2f& Collider2D::Position() const
	{
		return myPosition;
	}
	Shape2D Collider2D::GetShape()
	{
		return myShape;
	}

	void Collider2D::SetHitFunction(std::function<void()> aOnHitFunction)
	{
		myHitFunction = aOnHitFunction;
	}
	const std::function<void()> Collider2D::GetHitFunction()
	{
		return myHitFunction;
	}
}