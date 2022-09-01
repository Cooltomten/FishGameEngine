#pragma once


#include <Engine/Event/Event.h>
#include <string>

namespace Comp
{
	class Entity;
	class Component
	{
	public:
		Component(const std::string& aName);
		~Component() = default;

		virtual void Initialize(){}
		virtual void OnEvent(FGE::Event& aEvent) {}

		inline const std::string& GetName() const { return myName; }

	protected:
		Entity* myEntity;
	private:
		friend class Entity;
		std::string myName;	
		
	};
}