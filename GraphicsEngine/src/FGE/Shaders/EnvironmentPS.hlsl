#include "ShaderStructs.hlsli"
#include "PBRFunctions.hlsli"

DefferedPixelOutput main(DefferedVertexToPixel input)
{
    DefferedPixelOutput output;
    output.Color = float4(0, 0, 0, 0);
    
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
  
    const float3 pixelNormal = normalTexture.Sample(defaultSampler, input.UV).rgb;
    const float4 material = materialTexture.Sample(defaultSampler, input.UV);
    const float3 vertexNormal = vertexNormalTexture.Sample(defaultSampler, input.UV).rgb;
    const float4 worldPosition = worldPositionTexture.Sample(defaultSampler, input.UV);
    const float ambientOcclusion = ambientOclusionTexture.Sample(defaultSampler, input.UV).r;
    
    const float metalness = material.r;
    const float roughness = material.g;
    const float emissive = material.b;
    const float emissiveStr = material.a;
    
    
    const float3 toEye = normalize(FB_CameraTranslation.xyz - worldPosition.xyz);
    
    const float3 specularColor = lerp((float3) 0.04f, (float3) albedo, metalness);
    
    const float3 diffuseColor = lerp((float3) 0.00f, (float3) albedo, 1.0f - metalness);
    
    //IBL
    const float3 ambient = EvaluateAmbience(environmentTexture, pixelNormal, vertexNormal, toEye, roughness, ambientOcclusion, diffuseColor, specularColor);
    const float3 directLightning = EvaluateDirectionalLight(diffuseColor, specularColor, pixelNormal, roughness, LB_Color, LB_Intensity, -LB_Direction, toEye);
    
    //HEL?P
    float4 lightAccumulation = float4(ambient.r, ambient.g, ambient.b, 0) + float4(directLightning.r, directLightning.g, directLightning.b, 0) /*+emissive * emissiveStr*/;
    float4 aaaa = albedo * emissive * emissiveStr;
    float4 BBBBBB = lightAccumulation + aaaa;
        
    
    switch (FB_RenderMode)
    {
        default:
        case 0: //default
        {
            
                BBBBBB.w = 1;
                float4 color = BBBBBB;
            
                color.a = 1;
                output.Color = color;
                output.Color.a = 1;

       
            }
            break;
        case 1: //UV1
            output.Color = float4(input.UV.r, input.UV.g, 0, 1.0f);
            break;
        //case 2: //UV2
        //    output.Color = float4(input.UV2.r, input.UV2.g, 0, 1.0f);
        //    break;
        //case 3: //UV3
        //    output.Color = float4(input.UV3.r, input.UV3.g, 0, 1.0f);
        //    break;
        //case 4: //UV4
        //    output.Color = float4(input.UV4.r, input.UV4.g, 0, 1.0f);
        //    break;
        //case 5: //VertexColors1
        //    output.Color = float4(input.myVxColor.r, input.myVxColor.g, input.myVxColor.b, 1.0f);
        //    break;
        //case 6: //VertexColors2
        //    output.Color = float4(input.myVxColor2.r, input.myVxColor2.g, input.myVxColor.b, 1.0f);
        //    break;
        //case 7: //VertexColors3
        //    output.Color = float4(input.myVxColor3.r, input.myVxColor3.g, input.myVxColor.b, 1.0f);
        //    break;
        //case 8: //VertexColors4
        //    output.Color = float4(input.myVxColor4.r, input.myVxColor4.g, input.myVxColor.b, 1.0f);
        //    break;
        case 9: //VertexNormal
            {
                float3 debugNormal = vertexNormal;
                const float signedLength = (debugNormal.r + debugNormal.g + debugNormal.b) / 3;
                if (signedLength < 0)
                {
                    debugNormal = float3(1 - abs(debugNormal));
                }
                output.Color.rgb = debugNormal;
                output.Color.a = 1.0f;
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
                output.Color.rgb = debugNormal;
                output.Color.a = 1.0f;
            }
            break;
        case 11: //AlbedoMap
            output.Color.rgb = albedo.rgb;
            output.Color.a = 1.0f;
            break;
        case 12: //AmbientOcclusion
            output.Color.rgb = float3(ambientOcclusion, ambientOcclusion, ambientOcclusion);
            output.Color.a = 1.0f;
            break;
        case 13: //Roughness
            output.Color.rgb = float3(roughness, roughness, roughness);
            output.Color.a = 1.0f;
            break;
        case 14: //Metalness
            output.Color.rgb = float3(metalness, metalness, metalness);
            output.Color.a = 1.0f;
            break;
        case 15: //Emissiveness
            output.Color.rgb = float3(emissive, emissive, emissive);
            output.Color.a = 1.0f;
            break;
        case 16: //NormalMap 
            output.Color.rg = normalTexture.Sample(defaultSampler, input.UV).ag;
            output.Color.b = 1.0f;
            output.Color.a = 1.0f;
            break;
        case 17: //DiffuseLight
            output.Color.rgb = diffuseColor;
            output.Color.a = 1.0f;
            break;
        case 18: //AmbientLight
            output.Color.rgb = ambient;
            output.Color.a = 1.0f;
            break;
        case 19: //DiffuseNoAlbedo
            output.Color.rgb = lerp((float3) 0.00f, float3(1, 1, 1), 1.0f - metalness);
            break;
        case 20: //AmbientNoAlbedo
            output.Color.rgb = EvaluateAmbience(environmentTexture, pixelNormal, vertexNormal, toEye, roughness, ambientOcclusion, float3(1, 1, 1), float3(1, 1, 1));;
            break;
        
        
    }
    
    return output;
}