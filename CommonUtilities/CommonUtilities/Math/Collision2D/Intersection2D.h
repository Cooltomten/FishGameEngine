#pragma once
#include "Collider2D.h"
#include "Circle.h"
#include "Rectangle.h"
namespace CommonUtilities
{
	bool Intersects(Collider2D* aFirst, Collider2D* aSecond);
	bool IntersectionCircleCircle(const Circle& aFirst, const Circle& aSecond);
	bool IntersectionCircleAABB(const Circle& aFirst, const Rectangle& aSecond);
	bool IntersectionAABBAABB(const Rectangle& aFirst, const Rectangle& aSecond);

	

}