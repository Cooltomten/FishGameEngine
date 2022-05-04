#include "Timer.h"
#include "UtilityFunctions.hpp"
namespace CommonUtilities
{
	Timer::Timer()
	{
		myDeltaTime = 0;
		myStartTime = std::chrono::high_resolution_clock::now();
		myPrevTime = myStartTime;
		myTimeScale = 1;
		myMaxDelta = 10;
	}

	void Timer::Update()
	{
		myDeltaTime = std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now() - myPrevTime).count();
		myPrevTime = std::chrono::high_resolution_clock::now();
	}

	void Timer::SetTimeScale(float aValue)
	{
		myTimeScale = aValue;
	}

	float Timer::GetTimeScale() const
	{
		return myTimeScale;
	}

	float Timer::GetDeltaTime() const
	{
		return CommonUtilities::Clamp(myDeltaTime, 0.f, myMaxDelta) * myTimeScale;
	}

	float Timer::GetUnscaledDeltaTime()
	{
		return CommonUtilities::Clamp(myDeltaTime, 0.f, myMaxDelta);
	}

	double Timer::GetTotalTime() const
	{
		return std::chrono::duration<double, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now() - myStartTime).count();
	}
	void Timer::SetMaxDelta(float aMaxDelta)
	{
		myMaxDelta = aMaxDelta;
	}
};