#pragma once
#include "CommonUtilities/Math/Vector3.hpp"
#include "CommonUtilities/Math/Matrix4x4.hpp"
#include "FGE/Transform.h"
#include <vector>
#include <memory>

namespace CU = CommonUtilities;

namespace FGE
{
	class Component;
	class Entity
	{
	public:
	private:
		CU::Vector3f myPosition;
		CU::Vector3f myRotation;
		CU::Vector3f myScale;
		
		CU::Matrix4x4<float> myTransform;
		
		std::vector<std::shared_ptr<Component>> myComponents;
	};
}