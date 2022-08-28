#include "ShaderStructs.hlsli"

ParticleVertexData main( ParticleVertexData input ) 
{
	ParticleVertexData output;
	
	const float4 particleLocalPosition = input.Position;
	const float4 particleWorldPosition = mul(OB_ToWorld, particleLocalPosition);

	output.Position = particleWorldPosition;
	output.Color = input.Color;
	output.Velocity = input.Velocity;
	output.Scale = input.Scale;
	output.LifeTime = input.LifeTime;
	return output;
}