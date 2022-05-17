#include "ShaderStructs.hlsli" 
PixelOutput main(VertexToPixel input)
{
    PixelOutput output;
    const float average = (input.myVxColor.r + input.myVxColor.g + input.myVxColor.b) / 3.0f;
    output.myColor = average * float4(MB_Albedo, 1.0f);
    
    return output;
}