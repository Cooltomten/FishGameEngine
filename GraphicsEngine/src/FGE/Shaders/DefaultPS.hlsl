#include "ShaderStructs.hlsli" 
PixelOutput main(VertexToPixel input)
{
    PixelOutput output;
    
    
    
    float3 albedo = albedoTexture.Sample(defaultSampler, input.myUV).rgb;
    output.myColor.a = 1.0f;

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
    const float3 pixelNormal = normalize(mul(normalMap, TBN));
    
    //light calculations
    const float3 L = -1 * normalize(LB_Direction);
    const float3 N = pixelNormal;
    const float LdotN = saturate(dot(L, N));
    const float3 C = LB_Color;
    const float Ilight = LB_Intensity;
    
    const float3 Ipixel = LdotN * C * Ilight;
    
    const float3 diffuse = albedo * Ipixel;
    
    //IBL
    const float3 environment = environmentTexture.SampleLevel(defaultSampler, input.myNormal, 5).rgb;
    const float3 ambient = albedo * environment;
    
    //Default,
	//UV1,
	//UV2,
	//UV3,
	//UV4,
	//VertexColors1,
	//VertexColors2,
	//VertexColors3,
	//VertexColors4,
	//VertexNormal,
	//PixelNormal,
	//AlbedoMap,
	//NormalMap,
	//DiffuseLight,
	//AmbientLight,
    switch (FB_RenderMode)
    {
        default:
        case 0: //default
            output.myColor.rgb = saturate(diffuse + ambient);
            output.myColor.a = 1.f;
            break;
        case 1: //UV1
            output.myColor = float4(input.myUV.r, input.myUV.g, 0, 1.0f);
            break;
        case 2: //UV2
            output.myColor = float4(input.myUV2.r, input.myUV2.g, 0, 1.0f);
            break;
        case 3: //UV3
            output.myColor = float4(input.myUV3.r, input.myUV3.g, 0, 1.0f);
            break;
        case 4: //UV4
            output.myColor = float4(input.myUV4.r, input.myUV4.g, 0, 1.0f);
            break;
        case 5: //VertexColors1
            output.myColor = float4(input.myVxColor.r, input.myVxColor.g, input.myVxColor.b, 1.0f);
            break;
        case 6: //VertexColors2
            output.myColor = float4(input.myVxColor2.r, input.myVxColor2.g, input.myVxColor.b, 1.0f);
            break;
        case 7: //VertexColors3
            output.myColor = float4(input.myVxColor3.r, input.myVxColor3.g, input.myVxColor.b, 1.0f);
            break;
        case 8: //VertexColors4
            output.myColor = float4(input.myVxColor4.r, input.myVxColor4.g, input.myVxColor.b, 1.0f);
            break;
        case 9: //VertexNormal
            {
                float3 debugNormal = input.myNormal;
                const float signedLength = (debugNormal.r + debugNormal.g + debugNormal.b) / 3;
                if (signedLength < 0)
                {
                    debugNormal = float3(1 - abs(debugNormal));
                }
                output.myColor.rgb = debugNormal;
                output.myColor.a = 1.0f;
            }
            break;
        case 10: //PixelNormal
            {
         
                float3 debugNormal = pixelNormal;
                const float signedLength = (debugNormal.r + debugNormal.g + debugNormal.b) / 3;
                if (signedLength < 0)
                {
                    debugNormal = float3(1 - abs(debugNormal));
                }
                output.myColor.rgb = debugNormal;
                output.myColor.a = 1.0f;
            }
            break;
        case 11: //AlbedoMap
            output.myColor.rgb = albedo;
            output.myColor.a = 1.0f;
            break;
        case 12: //NormalMap 
            output.myColor.rg = normalTexture.Sample(defaultSampler, input.myUV).ag;
            output.myColor.b = 1.0f;
            output.myColor.a = 1.0f;
            break;
        case 13: //DiffuseLight
            output.myColor.rgb = diffuse;
            output.myColor.a = 1.0f;
            break;
        case 14: //AmbientLight
            output.myColor.rgb = ambient;
            output.myColor.a = 1.0f;
            break;
        case 15: //DiffuseNoAlbedo
            output.myColor.rgb = diffuse / albedo;
            break;
        case 16: //AmbientNoAlbedo
            output.myColor.rgb = environment;
            break;
        
        
    }
    
    return output;
}