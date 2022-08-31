#include "ShaderStructs.hlsli"
GBufferOutput main(VertexToPixel input)
{
	GBufferOutput output;
	
	const float4 albedo = albedoTexture.Sample(defaultSampler, input.myUV);
	
	if(albedo.a <= 0.05f)
    {
        discard;
        output.Albedo = float4(0, 0, 0, 0);
        output.Normal = float4(0, 0, 0, 0);
        output.Material = float4(0, 0, 0, 0);
        output.VertexNormal = float4(0, 0, 0, 0);
        output.WorldPosition = float4(0, 0, 0, 0);
        output.AmbientOcclusion = 0;
        return output;
    }
    
    float3 normalMap = normalTexture.Sample(defaultSampler, input.myUV).agb;
    float4 material = materialTexture.Sample(defaultSampler, input.myUV);
    
	//collect the material properties
    const float ambientOcclusion = normalMap.b;
    
    const float3x3 TBN = float3x3
    (
        normalize(input.myTangent),
        normalize(input.myBinormal),
        normalize(input.myNormal)
    );
    
    float3 pixelNormal = normalMap; 
    pixelNormal.z = 0;
    pixelNormal = 2.0f * pixelNormal - 1.0f;
    pixelNormal.z = sqrt(1 - saturate(pixelNormal.x + pixelNormal.x + pixelNormal.y + pixelNormal.y));
    pixelNormal = normalize(pixelNormal);
    pixelNormal = normalize(mul(pixelNormal, TBN));
    

    
    
    //fill output
    output.Albedo = albedo;
    output.Normal = float4(pixelNormal, 1.f);
    output.Material = material;
    output.VertexNormal = float4(normalize(input.myNormal), 1.f);
    output.WorldPosition = input.myVxPosition;
    output.AmbientOcclusion = ambientOcclusion;
    
    return output;
}