#pragma once
#include "..//Vector2.hpp"
#include <functional>


namespace CommonUtilities
{
	enum class Shape2D
	{
		Circle,
		Rectangle
	};
	class Collider2D
	{
	public:
		Collider2D() = default;
		virtual ~Collider2D() = 0;

		virtual bool IsInside(const Vector2f& aPoint) const = 0;

		Vector2f& Position();
		const Vector2f& Position() const;

		Shape2D GetShape();

		void SetHitFunction(std::function<void()> aOnHitFunction );
		const std::function<void()> GetHitFunction();
	protected:
		Vector2f myPosition;
		Shape2D myShape;

		std::function<void()> myHitFunction;
	};
}