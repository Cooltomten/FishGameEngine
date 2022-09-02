#include "Component.h"


namespace Comp
{

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