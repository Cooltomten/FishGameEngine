#pragma once
#include "CommonUtilities/Math/Vector4.hpp"
namespace CU = CommonUtilities;
namespace FGE
{
	struct Vertex
	{
		Vertex(float aX, float aY, float aZ, float aR, float aG, float aB, float aA)
		{
			Position = CU::Vector4f(aX, aY, aZ, 1.f);
			VertexColors[0] = CU::Vector4f(aR, aG, aB, aA);
		}
		CU::Vector4f Position = { 0,0,0,1 };
		CU::Vector4f VertexColors[4] =
		{
			{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0}
		};
	};
}
