#include "Math/Collision2D/Intersection2D.h"

bool CommonUtilities::Intersects(Collider2D* aFirst, Collider2D* aSecond)
{
	if (aFirst->GetShape() == Shape2D::Circle && aSecond->GetShape() == Shape2D::Circle)
	{
		return IntersectionCircleCircle(*reinterpret_cast<Circle*>(aFirst), *reinterpret_cast<Circle*>(aSecond));
	}
	else if (aFirst->GetShape() == Shape2D::Circle && aSecond->GetShape() == Shape2D::Rectangle)
	{
		return IntersectionCircleAABB(*reinterpret_cast<Circle*>(aFirst), *reinterpret_cast<Rectangle*>(aSecond));
	}
	else if (aFirst->GetShape() == Shape2D::Rectangle && aSecond->GetShape() == Shape2D::Circle)
	{
		return IntersectionCircleAABB(*reinterpret_cast<Circle*>(aSecond), *reinterpret_cast<Rectangle*>(aFirst));
	}
	else if (aFirst->GetShape() == Shape2D::Rectangle && aSecond->GetShape() == Shape2D::Rectangle)
	{
		return IntersectionAABBAABB(*reinterpret_cast<Rectangle*>(aSecond), *reinterpret_cast<Rectangle*>(aFirst));
	}
	return false;
}

bool CommonUtilities::IntersectionCircleCircle(const Circle& aFirst, const Circle& aSecond)
{
	return (aFirst.Position() - aSecond.Position()).LengthSqr() < (aFirst.Radius() + aSecond.Radius()) * (aFirst.Radius() + aSecond.Radius());
}

bool CommonUtilities::IntersectionCircleAABB(const Circle& aCircle, const Rectangle& aRectangle)
{
	float testX = aCircle.Position().x;
	float testY = aCircle.Position().y;

	CommonUtilities::Vector2f rMinPos = aRectangle.Position() - aRectangle.Size() / 2.f;
	CommonUtilities::Vector2f rMaxPos = aRectangle.Position() + aRectangle.Size() / 2.f;

	if (aCircle.Position().x < rMinPos.x)
	{
		testX = rMinPos.x;
	}
	else if (aCircle.Position().x > rMaxPos.x)
	{
		testX = rMaxPos.x;
	}

	if (aCircle.Position().x < rMinPos.y)
	{
		testY = rMinPos.y;
	}
	else if (aCircle.Position().x > rMaxPos.y)
	{
		testY = rMaxPos.y;
	}

	float distX = aCircle.Position().x - testX;
	float distY = aCircle.Position().y - testY;

	float distanceSqr = distX * distX + distY * distY;

	if (distanceSqr <= (aCircle.Radius() * aCircle.Radius()))
	{
		return true;
	}
	return false;
}

bool CommonUtilities::IntersectionAABBAABB(const Rectangle& aFirst, const Rectangle& aSecond)
{

	CommonUtilities::Vector2f firstMinPos = aFirst.Position() - aFirst.Size() / 2.f;
	CommonUtilities::Vector2f firstMaxPos = aFirst.Position() + aFirst.Size() / 2.f;

	CommonUtilities::Vector2f secondMinPos = aSecond.Position() - aSecond.Size() / 2.f;
	CommonUtilities::Vector2f secondMaxPos = aSecond.Position() + aSecond.Size() / 2.f;

	if (firstMinPos.x < secondMaxPos.x &&
		firstMaxPos.x > secondMinPos.x &&
		firstMinPos.y < secondMaxPos.y &&
		firstMaxPos.y > secondMinPos.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}
