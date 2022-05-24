#pragma once
#include "FGE/Event/Event.h"

#include <sstream>

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
}