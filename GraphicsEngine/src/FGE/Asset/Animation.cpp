#include "GraphicsEngine.pch.h"
#include "Animation.h"

#include "FGE/Asset/AnimatedMesh.h"
#include <CommonUtilities/UtilityFunctions.hpp>
#include <iostream>

std::vector<CU::Matrix4x4<float>> FGE::Animation::Sample(float aTime, Skeleton& aSkeleton,
	std::shared_ptr<Animation> aAnimationToBlendWith, float aBlendAlpha) const
{
	std::vector<CU::Matrix4x4<float>> result;
	result.resize(aSkeleton.Bones.size());

	float secondsPerFrame = 1 / FramesPerSecond;
	float frameTime = fmod(aTime, GetDuration()) / secondsPerFrame;
	int currentFrameIndex = static_cast<int>(std::floor(frameTime)) % (FrameCount - 1) + 1;
	uint32_t nextFrameIndex = (currentFrameIndex + 1);
	if (nextFrameIndex == FrameCount)
	{
		nextFrameIndex = 1;
	}
	float delta = frameTime - std::floor(frameTime);

	float secondsPerFrameBlend = 1 / aAnimationToBlendWith->FramesPerSecond;
	float frameTimeBlend = fmod(aTime, aAnimationToBlendWith->GetDuration()) / secondsPerFrameBlend;
	int currentFrameBlendIndex = static_cast<int>(std::floor(frameTimeBlend)) % (aAnimationToBlendWith->FrameCount - 1) + 1;
	uint32_t nextFrameBlendIndex = (currentFrameBlendIndex + 1);
	if (nextFrameBlendIndex == aAnimationToBlendWith->FrameCount)
	{
		nextFrameBlendIndex = 1;
	}
	float deltaBlend = frameTimeBlend - std::floor(frameTimeBlend);


	Frame currentFrame = Frames[currentFrameIndex];
	Frame nextFrame = Frames[nextFrameIndex];

	Frame currentFrameBlend = aAnimationToBlendWith->Frames[currentFrameBlendIndex];
	Frame nextFrameBlend = aAnimationToBlendWith->Frames[nextFrameBlendIndex];


	currentFrame.BlendWith(nextFrame, delta);
	currentFrameBlend.BlendWith(nextFrameBlend, deltaBlend);

	currentFrame.BlendWith(currentFrameBlend, aBlendAlpha);

	CalculateTransforms(currentFrame, aSkeleton,
		0, CU::Matrix4x4<float>(), result);
	return result;
}

void FGE::Animation::CalculateTransforms(const Frame& aFrame, Skeleton& aSkeleton, uint32_t aIndex, const CU::Matrix4x4<float>& aParentTransform, std::vector<CU::Matrix4x4<float>>& aTransforms) const
{
	const auto& bone = aSkeleton.Bones[aIndex];

	auto translation = aFrame.LocalTransforms[aIndex].Translation;
	auto rotation = aFrame.LocalTransforms[aIndex].Rotation;
	auto scale = aFrame.LocalTransforms[aIndex].Scale;

	DirectX::SimpleMath::Matrix dxtkLocalTransform = DirectX::SimpleMath::Matrix::Identity;
	dxtkLocalTransform *= DirectX::SimpleMath::Matrix::CreateScale(scale);
	dxtkLocalTransform *= DirectX::SimpleMath::Matrix::CreateFromQuaternion(rotation);
	dxtkLocalTransform *= DirectX::SimpleMath::Matrix::CreateTranslation(translation);

	CU::Matrix4x4<float> locTransform;
	locTransform = dxtkLocalTransform.m;

	CU::Matrix4x4<float> boneTransform = aParentTransform * locTransform.Transpose(locTransform);
	CU::Matrix4x4<float> result;
	result *= boneTransform;
	result *= bone.BindPoseInverse;
	aTransforms[aIndex] = result;

	//Dont calculate children with BindPoseInverse
	for (uint32_t i = 0; i < bone.Children.size(); ++i)
	{
		CalculateTransforms(aFrame, aSkeleton, bone.Children[i],
			boneTransform, aTransforms);
	}


}

float FGE::Animation::GetDuration() const
{
	float secondsPerFrame = 1 / FramesPerSecond;
	return (FrameCount - 1) * secondsPerFrame;
}

std::vector<CU::Matrix4x4<float>> FGE::BlendSpace::Sample(float aBlendAlpha, const Skeleton& aSkeleton, int aFrame) const
{

	std::vector<CU::Matrix4x4<float>> result;
	result.resize(aSkeleton.Bones.size());
	int firstAnimIndex = -1;
	for (int i = Values.size() - 2; i >= 0; i--)
	{
		if (aBlendAlpha >= Values[i])
		{
			firstAnimIndex = i;
			break;
		}
	}

	float delta = aBlendAlpha - Values[firstAnimIndex];
	//map delta to [0,1] range where 1 is the nex anim
	float deltaNormalized = delta / (Values[firstAnimIndex + 1] - Values[firstAnimIndex]);

	//if (deltaNormalized < 0.5)
	//{
	//	result = Animations[firstAnimIndex]->Frames;
	//}
	//else
	//{
	//	result = Animations[firstAnimIndex + 1]->Frames;
	//}


	return result;
}

void FGE::Animation::Frame::BlendWith(const Frame& aFrameToBlendWith, float aBlendAlpha)
{
	for (int i = 0; i < LocalTransforms.size(); ++i)
	{
		auto translationBlend = aFrameToBlendWith.LocalTransforms[i].Translation;
		auto rotationBlend = aFrameToBlendWith.LocalTransforms[i].Rotation;
		auto scaleBlend = aFrameToBlendWith.LocalTransforms[i].Scale;

		auto& translation = LocalTransforms[i].Translation;
		auto& rotation = LocalTransforms[i].Rotation;
		auto& scale = LocalTransforms[i].Scale;
		

		translation = translation.Lerp(translation, translationBlend, aBlendAlpha);
		rotation = rotation.Slerp(rotation, rotationBlend, aBlendAlpha);
		scale = scale.Lerp(scale, scaleBlend, aBlendAlpha);
		
	}
}
