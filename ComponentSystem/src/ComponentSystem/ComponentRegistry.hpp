#pragma once

#include <unordered_map>
#include <functional>
#include <memory>

//https://dxuuu.xyz/cpp-static-registration.html

#define REGISTER_COMPONENT(x)\
	static bool x ## _entry = Comp::ComponentRegistry::Add(x::GetFactoryName(), x::Create)

namespace Comp
{
	class Component;
	class ComponentRegistry
	{
	public:
		typedef std::function<std::shared_ptr<Component>()> FactoryFunction;
		typedef std::unordered_map<std::string, FactoryFunction> FactoryMap;

		static bool Add(const std::string& aName, FactoryFunction aFacFunc)
		{
			auto& map = GetFactoryMap();
			if (map.find(aName) != map.end()) {
				return false;
			}

			GetFactoryMap()[aName] = aFacFunc;
			return true;
		}

		static std::shared_ptr<Component> Create(const std::string& aName)
		{
			auto& map = GetFactoryMap();
			if (map.find(aName) == map.end())
			{
				return nullptr;
			}

			return map[aName]();
		}

		static FactoryMap& GetFactoryMap()
		{
			static FactoryMap map;
			return map;
		}
	private:

	};
}