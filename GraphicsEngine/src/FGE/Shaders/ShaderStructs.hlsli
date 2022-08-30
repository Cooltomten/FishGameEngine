#ifndef SHADER_STRUCTS
#define SHADER_STRUCTS
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
    
    float3 myTangent : TANGENT;
    float3 myBinormal : BINORMAL;
    float3 myNormal : NORMAL;
    
    uint4 myBoneIDs : BONEIDS;
    float4 myBoneWeights : BONEWEIGHTS;
    
};

cbuffer FrameBuffer: register(b0)
{
    float4x4 FB_ToView;
    float4x4 FB_ToProjection;
    float3 FB_CameraTranslation;
    unsigned int FB_RenderMode;
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

cbuffer LightBuffer : register(b3)
{
    float3 LB_Color;
    float LB_Intensity;
    float3 LB_Direction;
}

struct VertexToPixel
{
	float4 myPosition : SV_POSITION;
    float4 myVxPosition : POSITION;
    
    float2 myUV : TEXCOORD0;
    float2 myUV2 : TEXCOORD1;
    float2 myUV3 : TEXCOORD2;
    float2 myUV4 : TEXCOORD3;
    
    float4 myVxColor : COLOR0;
    float4 myVxColor2 : COLOR1;
    float4 myVxColor3 : COLOR2;
    float4 myVxColor4 : COLOR3;
    
    float3 myTangent : TANGENT;
    float3 myBinormal : BINORMAL;
    float3 myNormal : NORMAL;
};

struct DefferedVertexInput
{
    unsigned int myIndex : SV_VertexID;
};

struct DefferedVertexToPixel
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
};

struct PixelOutput
{
    float4 myColor : SV_TARGET;
};

struct GBufferOutput
{
    float4 Albedo : SV_TARGET0;
    float4 Normal : SV_TARGET1;
    float4 Material : SV_TARGET2;

    float4 VertexNormal : SV_TARGET3;
    
    float4 WorldPosition : SV_TARGET4;
    
    float AmbientOcclusion : SV_TARGET5;
};


struct ParticleVertexData
{
    float4 Position : POSITION;
	float4 Color : COLOR;
	float3 Velocity : VELOCITY;
	float3 Scale : SCALE;
	float LifeTime : LIFETIME;
};

struct ParticleGeometryToPixel
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float3 Velocity : VELOCITY;
	float LifeTime : LIFETIME;
	float2 UV : TEXCOORD0;
};

struct ParticlePixelOutput
{
	float4 Color : SV_TARGET;
};
struct DefferedPixelOutput
{
    float4 Color : SV_TARGET;
};

SamplerState defaultSampler : register(s0);

Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D materialTexture : register(t2);
Texture2D vertexNormalTexture : register(t3);
Texture2D worldPositionTexture: register(t4);
Texture2D ambientOclusionTexture: register(t5);


TextureCube environmentTexture : register(t10);

#endif