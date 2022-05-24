#include "GraphicsEngine.pch.h"
#include "Animation.h"

#include "FGE/Asset/AnimatedMesh.h"

std::vector<CU::Matrix4x4<float>> FGE::Animation::Sample(float aTime, Skeleton& aSkeleton) const
{
	std::vector<CU::Matrix4x4<float>> result;
	result.resize(aSkeleton.Bones.size());

	float secondsPerFrame = 1 / FramesPerSecond;
	float frameTime = aTime / secondsPerFrame;
	int frame = std::floor(frameTime);

	if(frame >= Frames.size())
	{
		frame = 0;
	}


	CalculateTransforms(frame, aSkeleton, 0, CU::Matrix4x4<float>(), result);
	return result;
}

void FGE::Animation::CalculateTransforms(uint32_t aCurrentFrame, Skeleton& aSkeleton, uint32_t aIndex,
	const CU::Matrix4x4<float>& aParentTransform, std::vector<CU::Matrix4x4<float>>& aTransforms) const
{
	const auto& bone = aSkeleton.Bones[aIndex];
	CU::Matrix4x4<float> locTransform = Frames[aCurrentFrame].LocalTransforms[aIndex];

	CU::Matrix4x4<float> boneTransform = aParentTransform * locTransform.Transpose(locTransform);
	CU::Matrix4x4<float> result;
	result *= boneTransform;
	result *= bone.BindPoseInverse;
	aTransforms[aIndex] = result;
	
	//Dont calculate children with BindPoseInverse
	for (uint32_t i = 0; i < bone.Children.size(); ++i)
	{
		CalculateTransforms(aCurrentFrame, aSkeleton, bone.Children[i], boneTransform, aTransforms);
	}


}
