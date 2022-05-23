#pragma once
#include <string>

namespace Comp
{
	class Entity;
	class Component
	{
	public:
		Component(const std::string& aName);
		~Component() = default;

		void OnEvent();

		inline const std::string& GetName() const { return myName; }

	private:
		friend class Entity;
		std::string myName;	
		Entity* myEntity;
		
	};
}