#include "ShaderStructs.hlsli"

VertexToPixel main(VertexInput input)
{
	
    float4 vertexObjectPosition = input.myPosition;
    
    if (OB_HasBones)
    {
        vertexObjectPosition = 0;
    
        vertexObjectPosition += input.myBoneWeights.x * mul(input.myPosition, OB_BoneData[input.myBoneIDs.x]);
        vertexObjectPosition += input.myBoneWeights.y * mul(input.myPosition, OB_BoneData[input.myBoneIDs.y]);
        vertexObjectPosition += input.myBoneWeights.z * mul(input.myPosition, OB_BoneData[input.myBoneIDs.z]);
        vertexObjectPosition += input.myBoneWeights.w * mul(input.myPosition, OB_BoneData[input.myBoneIDs.w]);
    }
    
    const float4 vertexWorldPosition = mul(OB_ToWorld, vertexObjectPosition);

    const float4 vertexViewPosition = mul(FB_ToView, vertexWorldPosition);
    
    const float4 vertexProjectionPosition = mul(FB_ToProjection, vertexViewPosition);
    
    
    VertexToPixel result;
    result.myPosition = vertexProjectionPosition;
    
    result.myVxColor = input.myVxColor;
    result.myVxColor2 = input.myVxColor2;
    result.myVxColor3 = input.myVxColor3;
    result.myVxColor4 = input.myVxColor4;
    
    return result;
}