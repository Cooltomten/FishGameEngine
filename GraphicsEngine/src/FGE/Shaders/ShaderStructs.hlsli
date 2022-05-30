struct VertexInput
{
	float4 myPosition : POSITION;
	float4 myVxColor : COLOR0;
	float4 myVxColor2 : COLOR1;
	float4 myVxColor3 : COLOR2;
	float4 myVxColor4 : COLOR3;
    
    float2 myUV : TEXCOORD0;
    float2 myUV2 : TEXCOORD1;
    float2 myUV3 : TEXCOORD2;
    float2 myUV4 : TEXCOORD3;
    
    uint4 myBoneIDs : BONEIDS;
    float4 myBoneWeights : BONEWEIGHTS;
    
};

cbuffer FrameBuffer: register(b0)
{
    float4x4 FB_ToView;
    float4x4 FB_ToProjection;
}

cbuffer ObjectBuffer: register(b1)
{
    float4x4 OB_ToWorld;
    float4x4 OB_BoneData[128];
    bool OB_HasBones;
}

cbuffer MaterialBuffer: register(b2)
{
    float3 MB_Albedo;
    float padding;
}

struct VertexToPixel
{
	float4 myPosition : SV_POSITION;
    
    float2 myUV : TEXCOORD0;
    float2 myUV2 : TEXCOORD1;
    float2 myUV3 : TEXCOORD2;
    float2 myUV4 : TEXCOORD3;
    
    float4 myVxColor : COLOR0;
    float4 myVxColor2 : COLOR1;
    float4 myVxColor3 : COLOR2;
    float4 myVxColor4 : COLOR3;
};


struct PixelOutput
{
    float4 myColor : SV_TARGET;
};

