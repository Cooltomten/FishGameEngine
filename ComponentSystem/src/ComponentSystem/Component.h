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
		Parameter(ParameterType aParamType, const std::string& aName, void* aValue)
			: myType(aParamType)
			, myName(aName)
			, myValue(aValue)
		{
		}

		ParameterType myType;
		std::string myName;
		void* myValue;
	};

	std::string ParameterTypeToString(ParameterType aType);
	ParameterType StringToParameterType(const std::string& aType);
	
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