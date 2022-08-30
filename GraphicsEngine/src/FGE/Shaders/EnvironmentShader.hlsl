#include "ShaderStructs.hlsli"
#include "PBRFunctions.hlsli"

DefferedPixelOutput main(DefferedVertexToPixel input) : SV_TARGET
{
    DefferedPixelOutput output;
    
    const float4 albedo = albedoTexture.Sample(defaultSampler, input.UV);

    
    if (albedo.a == 0)
    {
        discard;
        
        DefferedPixelOutput v;
        v.Color = float4(0, 0, 0, 0);
        return v;
    }
    
  // normal 
  // material
  // vertexNormal
  // worldPosition
  // ambientOcclusion
  
    const float3 normal = normalTexture.Sample(defaultSampler, input.UV);
    const float4 material = materialTexture.Sample(defaultSampler, input.UV);
    const float3 vertexNormal = vertexNormalTexture.Sample(defaultSampler, input.UV);
    const float4 worldPosition = worldPositionTexture.Sample(defaultSampler, input.UV);
    const float ambientOcclusion = ambientOclusionTexture.Sample(defaultSampler, input.UV);
    
    
    
    const float3 toEye = normalize(FB_CameraTranslation.xyz - input.myVxPosition.xyz);
    
    const float3 specularColor = lerp((float3) 0.04f, albedo, metalness);
    
    const float3 diffuseColor = lerp((float3) 0.00f, albedo, 1.0f - metalness);
    
    //IBL
    const float3 ambient = EvaluateAmbience(environmentTexture,, vertexNormal, toEye, roughness, ambientOcclusion, diffuseColor, specularColor);
    const float3 directLightning = EvaluateDirectionalLight(diffuseColor, specularColor, pixelNormal, roughness, LB_Color, LB_Intensity, -LB_Direction, toEye);
    
    
    return output;
}