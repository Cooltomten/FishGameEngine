#pragma once
#include "FGE/Asset/Asset.h"
#include <CommonUtilities/Math/Vector.hpp>

namespace CU = CommonUtilities;

namespace FGE
{
	
class EmitterSettingsData : public Asset
{
public:
	static AssetType GetStaticType() { return AssetType::EmitterSettingsData; }
	inline AssetType GetAssetType() const override { return GetStaticType(); }
	
	//amount of particles per second
	float ParticlesPerSecond = 10;
	
	//Life Time in seconds
	float LifeTime = 5;

	//Initial velocity of the particle
	CU::Vector3f StartVelocity = { 0,0,0 };
	
	//Initial Size of the particle
	float StartScale = 1;

	//Initial Color of the particle
	CU::Vector4f StartColor = { 1,1,1,1 };
	
};
}