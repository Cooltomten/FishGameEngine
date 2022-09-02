#pragma once


#include <Engine/Event/Event.h>
#include <string>
#include <vector>


namespace Comp
{
	enum class ParameterType
	{
		Int,
		Float,
		String,
		Bool,
		Vec2,
		Vec3,
		Vec4,
		Color
	};

	struct Parameter
	{
		ParameterType myType;
		std::string myName;
		void* myValue;
	};
	class Entity;
	class Component
	{
	public:
		Component(const std::string& aName);
		~Component() = default;

		virtual void Initialize() {}
		virtual void OnEvent(FGE::Event& aEvent) {}
		void InitializeParameters(const std::initializer_list<Parameter>& aParameters);
		const std::vector<Parameter>& GetParameters();

		inline const std::string& GetName() const { return myName; }

	protected:
		Entity* myEntity;
	private:
		std::vector<Parameter> myParameters;
		friend class Entity;
		std::string myName;

	};
}