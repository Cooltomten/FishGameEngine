#include "ShaderStructs.hlsli" 
PixelOutput main(VertexToPixel input)
{
    PixelOutput output;
    //const float average = (input.myVxColor.r + input.myVxColor.g + input.myVxColor.b) / 3.0f;
    //output.myColor = average * float4(MB_Albedo, 1.0f);
    
    
    
    //output.myColor.rgb = albedoTexture.Sample(defaultSampler, input.myUV).rgb;
    //output.myColor.a = 1.0f;
    
    
    //output.myColor = float4(input.myUV.r, input.myUV.g, 0, 1.0f);
    

    
    
    const float3x3 TBN = float3x3
    (
        normalize(input.myTangent),
        normalize(input.myBinormal),
        normalize(input.myNormal)
    );
    
    float3 normalMap = normalTexture.Sample(defaultSampler, input.myUV).agr;
    normalMap.z = 0;
    
    normalMap = 2.0f * normalMap - 1.0f;
    
    normalMap.z = sqrt(1 - saturate(normalMap.x + normalMap.x + normalMap.y + normalMap.y));
    
    normalMap = normalize(normalMap);
    const float3 pixelNormal = normalize(mul( normalMap, TBN ));
    
    float3 debugNormal = pixelNormal   ;
    const float signedLength = (debugNormal.r + debugNormal.g + debugNormal.b) / 3;
    if (signedLength < 0)
    {
        debugNormal = float3(1 - abs(debugNormal));
    }
    output.myColor.rgb = debugNormal;
    output.myColor.a = 1.0f;
    
    
    
    return output;
}