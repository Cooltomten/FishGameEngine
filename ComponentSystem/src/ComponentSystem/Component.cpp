#include "Component.h"


namespace Comp
{
	std::string ParameterTypeToString(ParameterType aType)
	{		
		std::string result = "";
		switch (aType)
		{
		case ParameterType::Int:
			result = "Int";
			break;
		case ParameterType::Float:
			result = "Float";
			break;
		case ParameterType::String:
			result = "String";
			break;
		case ParameterType::Bool:
			result = "Bool";
			break;
		case ParameterType::Vec2:
			result = "Vec2";
			break;
		case ParameterType::Vec3:
			result = "Vec3";
			break;
		case ParameterType::Vec4:
			result = "Vec4";
			break;
		case ParameterType::Color:
			result = "Color";
			break;
		}
		return result;
	}

	ParameterType StringToParameterType(const std::string& aType)
	{
		ParameterType result = ParameterType::Int;
		if (aType == "Int")
		{
			result = ParameterType::Int;
		}
		else if (aType == "Float")
		{
			result = ParameterType::Float;
		}
		else if (aType == "String")
		{
			result = ParameterType::String;
		}
		else if (aType == "Bool")
		{
			result = ParameterType::Bool;
		}
		else if (aType == "Vec2")
		{
			result = ParameterType::Vec2;
		}
		else if (aType == "Vec3")
		{
			result = ParameterType::Vec3;
		}
		else if (aType == "Vec4")
		{
			result = ParameterType::Vec4;
		}
		else if (aType == "Color")
		{
			result = ParameterType::Color;
		}
		return result;
	}

	Comp::Component::Component(const std::string& aName)
		: myName(aName)
	{
		myEntity = nullptr;
	}

	void Comp::Component::InitializeParameters(const std::initializer_list<Parameter>& aParameters)
	{
		myParameters = aParameters;
	}

	const std::vector<Parameter>& Comp::Component::GetParameters()
	{
		return myParameters;
	}



}