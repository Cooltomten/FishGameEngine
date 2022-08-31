#include "ShaderStructs.hlsli"



DefferedVertexToPixel main(DefferedVertexInput input)
{
    const float4 pos[3] =
    {
        float4(-1.0, -1.0, 0.0, 1.0),
        float4(-1.0, 3.0, 0.0, 1.0),
        float4(3.0, -1.0, 0.0, 1.0)
    };
    
    const float2 uv[3] =
    {
        float2(0.0, 1.0),
        float2(0.0, -1.0),
        float2(2.0, 1.0)
    };
    
    DefferedVertexToPixel v;
    v.Position = pos[input.myIndex];
    v.UV = uv[input.myIndex];
    return v;
}