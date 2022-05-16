#include "ShaderStructs.hlsli" 
PixelOutput main(VertexToPixel input)
{
	PixelOutput output;
    output.myColor = input.myVxColor;
    return output;
}