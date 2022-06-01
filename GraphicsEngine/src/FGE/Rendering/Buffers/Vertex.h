#pragma once
#include "CommonUtilities/Math/Vector4.hpp"
#include "CommonUtilities/Math/Vector3.hpp"
#include "CommonUtilities/Math/Vector2.hpp"
namespace CU = CommonUtilities;
namespace FGE
{
	struct Vertex
	{
		Vertex(float aPosition[4],float someVertexColors[4][4],float someUVs[4][2], float aNormal[3], float aTangent[3], float aBinormal[3], unsigned int someBoneIDs[4], float someBoneWeights[4])
		{
			Position = CU::Vector4f(aPosition[0], aPosition[1], aPosition[2], aPosition[3]);
			for (int i = 0; i < 4; ++i)
			{
				VertexColors[i] = CU::Vector4f(someVertexColors[i][0], someVertexColors[i][1], someVertexColors[i][2], someVertexColors[i][3]);
			}
			
			for (int i = 0; i < 4; ++i)
			{
				UVs[i] = CU::Vector2f(someUVs[i][0], someUVs[i][1]);
			}
			
			Normal = CU::Vector3f(aNormal[0], aNormal[1], aNormal[2]);
			Tangent = CU::Vector3f(aTangent[0], aTangent[1], aTangent[2]);
			Binormal = CU::Vector3f(aBinormal[0], aBinormal[1], aBinormal[2]);
			
			BoneIDs = CU::Vector4<unsigned int>(someBoneIDs[0], someBoneIDs[1], someBoneIDs[2], someBoneIDs[3]);
			BoneWeights = CU::Vector4f(someBoneWeights[0], someBoneWeights[1], someBoneWeights[2], someBoneWeights[3]);
		}
		
		Vertex(CU::Vector4f aPosition, CU::Vector4f aVertexColor)
		{
			Position = aPosition;
			VertexColors[0] = aVertexColor;
		}
		Vertex(CU::Vector4f aPosition, CU::Vector4f aVertexColor, CU::Vector2f aUV)
		{
			Position = aPosition;
			VertexColors[0] = aVertexColor;
			UVs[0] = aUV;
		}
		Vertex(CU::Vector4f aPosition, CU::Vector4f aVertexColor, CU::Vector2f aUV, CU::Vector3f aNormal,  CU::Vector3f aTangent, CU::Vector3f aBinormal)
		{
			Position = aPosition;
			VertexColors[0] = aVertexColor;
			UVs[0] = aUV;
			Normal = aNormal;
			Binormal = aBinormal;
			Tangent = aTangent;
		}

		CU::Vector4f Position = { 0,0,0,1 };
		CU::Vector4f VertexColors[4] =
		{
			{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0},
			{0,0,0,0}
		};
		CU::Vector2f UVs[4] = { { 0,0 },{ 0,0 },{ 0,0 },{ 0,0 } };

		CU::Vector3f Normal = { 0,0,0 };
		CU::Vector3f Tangent = { 0,0,0 };
		CU::Vector3f Binormal = { 0,0,0 };
		
		CU::Vector4<unsigned int> BoneIDs = { 0,0,0,0 };
		CU::Vector4f BoneWeights = { 0,0,0,0 };
		
	};
}
