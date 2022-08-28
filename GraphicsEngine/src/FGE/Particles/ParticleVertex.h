#pragma once
#include <CommonUtilities/Math/Vector.hpp>
namespace CU = CommonUtilities;

namespace FGE
{
	struct ParticleVertex
	{
		CU::Vector4f Position;
		CU::Vector4f Color;
		CU::Vector3f Velocity;
		CU::Vector3f Scale;
		float LifeTime;
	};
}