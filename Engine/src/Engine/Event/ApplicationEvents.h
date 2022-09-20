#pragma once
#include "Engine/Event/Event.h"

#include <sstream>
#include <array>

namespace FGE
{
	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent(float aTimeStep) : myTimeStep(aTimeStep) {}
		~AppUpdateEvent() {}

		float GetTimeStep() const { return myTimeStep; }


		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AppUpdateEvent: " << myTimeStep;
			return ss.str();
		}
		
		EVENT_CLASS_TYPE(AppUpdate)

	private:
		float myTimeStep;
	};

	class AppRenderEvent : public Event
	{
		public:
		AppRenderEvent() {}
		~AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
	};

	class ClearColorChangedEvent : public Event
	{
	public:
		ClearColorChangedEvent(std::array<float, 4> aClearColor) : myClearColor(aClearColor) {}
		~ClearColorChangedEvent() {}

		const std::array<float, 4>& GetClearColor() const { return myClearColor; }

		EVENT_CLASS_TYPE(ClearColorChanged)
			
	private:
		std::array<float, 4> myClearColor;
	};
}