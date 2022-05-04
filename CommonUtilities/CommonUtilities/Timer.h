#pragma once
#include <chrono>
namespace CommonUtilities
{
	class Timer
	{
	public:
		Timer();
		Timer(const Timer& aTimer) = delete;
		Timer& operator=(const Timer& aTimer) = delete;
		void Update();
		void SetTimeScale(float aValue);
		float GetTimeScale() const;
		float GetDeltaTime() const;
		float GetUnscaledDeltaTime();
		double GetTotalTime() const;
		void SetMaxDelta(float aMaxDelta);
	private:
		std::chrono::high_resolution_clock::time_point myStartTime;
		std::chrono::high_resolution_clock::time_point myPrevTime;
		float myTimeScale;
		float myDeltaTime;
		float myMaxDelta;
	};
};

