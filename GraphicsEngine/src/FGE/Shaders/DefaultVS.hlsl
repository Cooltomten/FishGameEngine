#include "ShaderStructs.hlsli"

VertexToPixel main(VertexInput input)
{
	
    float4x4 skinningMatrix =
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    
    if (OB_HasBones)
    {
        skinningMatrix = 0;
    
        skinningMatrix += mul(input.myBoneWeights.x , OB_BoneData[input.myBoneIDs.x]);
        skinningMatrix += mul(input.myBoneWeights.y , OB_BoneData[input.myBoneIDs.y]);
        skinningMatrix += mul(input.myBoneWeights.z , OB_BoneData[input.myBoneIDs.z]);
        skinningMatrix += mul(input.myBoneWeights.w , OB_BoneData[input.myBoneIDs.w]);
    }
    
    const float4 vertexWorldPosition = mul(OB_ToWorld, mul(input.myPosition, skinningMatrix));
    
    const float4 vertexViewPosition = mul(FB_ToView, vertexWorldPosition);
    
    const float4 vertexProjectionPosition = mul(FB_ToProjection, vertexViewPosition);

    VertexToPixel result;
    //Normals
    const float3x3 worldNormalRotation = (float3x3) OB_ToWorld;
    const float3x3 skinNormalRotation = (float3x3) skinningMatrix;
    
    result.myTangent = mul(worldNormalRotation, mul(input.myTangent, skinNormalRotation));
    result.myNormal = mul(worldNormalRotation, mul(input.myNormal, skinNormalRotation));
    result.myBinormal = mul(worldNormalRotation, mul(input.myBinormal, skinNormalRotation));
    
    
    result.myPosition = vertexProjectionPosition;
    
    result.myVxColor = input.myVxColor;
    result.myVxColor2 = input.myVxColor2;
    result.myVxColor3 = input.myVxColor3;
    result.myVxColor4 = input.myVxColor4;
    
    result.myUV = input.myUV;
    result.myUV2 = input.myUV2;
    result.myUV3 = input.myUV3;
    result.myUV4 = input.myUV4;
    
    return result;
}