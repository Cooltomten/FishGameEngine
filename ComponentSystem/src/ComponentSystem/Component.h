#pragma once


#include <FGE/Event/Event.h>
#include <string>

namespace Comp
{
	class Entity;
	class Component
	{
	public:
		Component(const std::string& aName);
		~Component() = default;

		virtual void OnEvent(const FGE::Event& aEvent);

		inline const std::string& GetName() const { return myName; }

	private:
		friend class Entity;
		std::string myName;	
		Entity* myEntity;
		
	};
}