#include "ShaderStructs.hlsli" 
#include "PBRFunctions.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;
    
    
    

    const float3x3 TBN = float3x3
    (
        normalize(input.myTangent),
        normalize(input.myBinormal),
        normalize(input.myNormal)
    );
    
    //sample textures
    float3 albedo = albedoTexture.Sample(defaultSampler, input.myUV).rgb;
    float3 normalMap = normalTexture.Sample(defaultSampler, input.myUV).agb;
	float4 material = materialTexture.Sample(defaultSampler, input.myUV);
    
	//collect the material properties
    const float ambientOcclusion = normalMap.b;
    const float metalness = material.r;
	const float roughness = material.g;
	const float emissive = material.b;
	const float emissiveStr = material.a;
    
    
     float3 pixelNormal = normalMap;
    pixelNormal.z = 0;
    pixelNormal = 2.0f * pixelNormal - 1.0f;
    pixelNormal.z = sqrt(1 - saturate(pixelNormal.x+ pixelNormal.x + pixelNormal.y + pixelNormal.y));
    pixelNormal = normalize(pixelNormal);
    pixelNormal = normalize(mul(pixelNormal, TBN));
    
    
    const float3 toEye = normalize(FB_CameraTranslation.xyz - input.myVxPosition.xyz);
    
    const float3 specularColor = lerp((float3) 0.04f, albedo, metalness);
    
    const float3 diffuseColor = lerp((float3) 0.00f, albedo, 1.0f - metalness);
    
    //IBL
    const float3 ambient = EvaluateAmbience(environmentTexture, pixelNormal, input.myNormal, toEye, roughness, ambientOcclusion, diffuseColor, specularColor);
    const float3 directLightning = EvaluateDirectionalLight(diffuseColor, specularColor, pixelNormal, roughness, LB_Color, LB_Intensity, -LB_Direction, toEye);
    
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
    //AmbientOcclusion,
    //Roughness,
    //Metalness,
    //Emissiveness,
	//NormalMap,
	//DiffuseLight,
	//AmbientLight,
    switch (FB_RenderMode)
    {
        default:
        case 0: //default
            output.myColor.rgb = ambient + directLightning;
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
		case 12: //AmbientOcclusion
			output.myColor.rgb = float3(ambientOcclusion, ambientOcclusion, ambientOcclusion);
			output.myColor.a = 1.0f;
			break;
		case 13: //Roughness
			output.myColor.rgb = float3(roughness, roughness, roughness);
			output.myColor.a = 1.0f;
			break;
		case 14: //Metalness
			output.myColor.rgb = float3(metalness, metalness, metalness);
			output.myColor.a = 1.0f;
			break;
		case 15: //Emissiveness
			output.myColor.rgb = float3(emissive, emissive, emissive);
			output.myColor.a = 1.0f;
			break;
        case 16: //NormalMap 
            output.myColor.rg = normalTexture.Sample(defaultSampler, input.myUV).ag;
            output.myColor.b = 1.0f;
            output.myColor.a = 1.0f;
            break;
        case 17: //DiffuseLight
            output.myColor.rgb = diffuseColor;
            output.myColor.a = 1.0f;
            break;
        case 18: //AmbientLight
            output.myColor.rgb = ambient;
            output.myColor.a = 1.0f;
            break;
        case 19: //DiffuseNoAlbedo
            output.myColor.rgb = lerp((float3) 0.00f, float3(1,1,1), 1.0f - metalness);
            break;
        case 20: //AmbientNoAlbedo
            output.myColor.rgb = EvaluateAmbience(environmentTexture, pixelNormal, input.myNormal, toEye, roughness, ambientOcclusion, float3(1, 1, 1), float3(1, 1, 1));;
            break;
        
        
    }
    
    float3 finalRgb = LinearToGamma(output.myColor.rgb);
    output.myColor.rgb = finalRgb;
    return output;
}