#pragma once
#include <cassert>
namespace CommonUtilities
{
	template<typename T> T Max(T aFirst, T aSecond)
	{
		if (aFirst < aSecond)
		{
			return aSecond;
		}
		else
		{
			return aFirst;
		}
	}
	template<typename T> T Min(T aFirst, T aSecond)
	{
		if (aFirst < aSecond)
		{
			return aFirst;
		}
		else
		{
			return aSecond;
		}
	}
	template<typename T> T Abs(T aValue)
	{
		if (aValue < 0)
		{
			return -aValue;
		}
		else
		{
			return aValue;
		}
	}

	template<typename T> T Clamp(T aValue, T aMin, T aMax)
	{
		assert(aMin <= aMax && "ERROR! min value is larger than max value.");
		if (aValue < aMin)
		{
			return aMin;
		}
		else if (aMax < aValue)
		{
			return aMax;
		}
		else
		{
			return aValue;
		}
	}

	template<typename T> T Lerp(T aStartValue, T aEndValue, float aPercent)
	{
		return (aStartValue + aPercent * (aEndValue - aStartValue));
	}

	template<typename T> void Swap(T& aFirst, T& aSecond)
	{
		T temp = aFirst;
		aFirst = aSecond;
		aSecond = temp;
	}
}