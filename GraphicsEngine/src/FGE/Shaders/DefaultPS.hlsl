#include "ShaderStructs.hlsli" 
PixelOutput main(VertexToPixel input)
{
    PixelOutput output;
    const float average = (input.myVxColor.r + input.myVxColor.g + input.myVxColor.b) / 3.0f;
    output.myColor = average * float4(MB_Albedo, 1.0f);
    
    
    output.myColor = float4(input.myUV.r, input.myUV.g, 0, 1.0f);
    return output;
}