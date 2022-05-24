#pragma once

#define BIND_EVENT_FN(fn) (std::bind(&fn, this, std::placeholders::_1))
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
#include <string>
#include <ostream>
namespace FGE
{

	enum EventType : uint32_t
	{
		None = 0,
		//WindowClose,
		//WindowResize,
		//WindowFocus,
		//WindowLostFocus,
		//WindowMoved,

		//AppEarlyUpdate,
		AppUpdate,
		//AppLateUpdate,
		AppRender,

		//KeyPressed,
		//KeyReleased,
		//KeyTyped,
	};
	


	class Event
	{
	public:
		virtual ~Event() = default;
		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }
		
	protected:
		EventType myType = None;
	};

	class EventDispatcher
	{
	public: 
		EventDispatcher(Event& aEvent) : myEvent(aEvent) {}
		virtual ~EventDispatcher() = default;

		template<typename T, typename F>
		bool Dispatch(const F& aFunction)
		{
			if (myEvent.GetEventType() == T::GetStaticType())
			{
				myEvent.Handled = aFunction(static_cast<T&>(myEvent));
				return true;
			}
			return false;
		}
		
	private:
		Event& myEvent;
	};
	
	inline std::ostream& operator<<(std::ostream& aStream, const Event& aEvent)
	{
		return aStream << aEvent.ToString();
	}

	
}