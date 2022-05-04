#include "Math/Collision2D/Rectangle.h"
#include "Math/LineVolume.hpp"
#include "Math/Matrix3x3.hpp"
namespace CommonUtilities
{
	Rectangle::Rectangle(const Vector2f& aPosition, const Vector2f& aSize, float aRotation)
	{
		myPosition = aPosition;
		mySize = aSize;
		myShape = Shape2D::Rectangle;
		myRotation = aRotation;
	}

	Rectangle::~Rectangle()
	{
	}

	bool Rectangle::IsInside(const Vector2f& aPosition) const
	{
		if (myRotation > -0.001 && myRotation < 0.001)
		{
			float minX = myPosition.x - mySize.x;
			if ((myPosition.x - (mySize.x / 2)) > aPosition.x) return false; // pos is left of rect
			if ((myPosition.y - (mySize.y / 2)) > aPosition.y) return false; // pos is above rect
			if ((myPosition.x + (mySize.x / 2)) < aPosition.x) return false; // pos is right of rect
			if ((myPosition.y + (mySize.y / 2)) < aPosition.y) return false; // pos is below rect


			return true; // pos is inside rect
		}
		else
		{
			Matrix3x3<float> rotZMat = Matrix3x3<float>::CreateRotationAroundZ(myRotation);

			Vector3f relativeMinPos = { -mySize.x / 2, -mySize.y / 2 , 0 };
			Vector3f relativeMaxPos = { mySize.x / 2, mySize.y / 2 , 0 };

			Vector3f posA = { -mySize.x / 2, -mySize.y / 2 , 0 };
			Vector3f posB = { mySize.x / 2, -mySize.y / 2 , 0 };
			Vector3f posC = { mySize.x / 2, mySize.y / 2 , 0 };
			Vector3f posD = { -mySize.x / 2, mySize.y / 2 , 0 };

			posA = posA * rotZMat;
			posB = posB * rotZMat;
			posC = posC * rotZMat;
			posD = posD * rotZMat;

			LineVolume<float> vol;
			vol.AddLine(Line<float>({ posA.x, posA.y }, { posB.x, posB.y }));
			vol.AddLine(Line<float>({ posB.x, posB.y }, { posC.x, posC.y }));
			vol.AddLine(Line<float>({ posC.x, posC.y }, { posD.x, posD.y }));
			vol.AddLine(Line<float>({ posD.x, posD.y}, { posA.x, posA.y }));

			return vol.IsInside(aPosition);
		}

	}
	Vector2f& Rectangle::Size()
	{
		return mySize;
	}
	const Vector2f Rectangle::Size() const
	{
		return mySize;
	}
	float& Rectangle::Rotation()
	{
		return myRotation;
	}
	const float& Rectangle::Rotation() const
	{
		return 0.0f;
	}
}