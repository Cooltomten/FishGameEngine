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

	
	float originalToBlendFPS = (FrameCount - 1) * aAnimationToBlendWith->FramesPerSecond / aAnimationToBlendWith->FrameCount;

	float secondsPerFrame = 1 / CU::Lerp(FramesPerSecond, originalToBlendFPS, aBlendAlpha);
	float duration = (FrameCount -1) * secondsPerFrame;
	float frameTime = fmod(aTime , duration) / secondsPerFrame;
	int frame = static_cast<int>(std::floor(frameTime)) % (FrameCount-1) + 1;
	uint32_t nextFrame = (frame + 1);
	if(nextFrame == FrameCount)
	{
		nextFrame = 1;
	}
	float delta = frameTime - std::floor(frameTime);


	float blendToOriginalFPS = (aAnimationToBlendWith->FrameCount - 1) * FramesPerSecond / FrameCount;
	
	float secondsPerFrameBlend = 1 / CU::Lerp(aAnimationToBlendWith->FramesPerSecond, blendToOriginalFPS, 1-aBlendAlpha);
	float durBlend =  (aAnimationToBlendWith->FrameCount-1) * secondsPerFrameBlend;
	float frameTimeBlend = fmod(aTime,durBlend) / secondsPerFrameBlend;
	int frameBlend = static_cast<int>(std::floor(frameTimeBlend)) % (aAnimationToBlendWith->FrameCount - 1) + 1;
	uint32_t nextFrameBlend = (frameBlend + 1) ;
	if(nextFrameBlend == aAnimationToBlendWith->FrameCount)
	{
		nextFrameBlend = 1;
	}
	float deltaBlend = frameTimeBlend - std::floor(frameTimeBlend);

	
	CalculateTransforms(frame, nextFrame, frameBlend, nextFrameBlend,
		delta, deltaBlend, aBlendAlpha, aSkeleton,
		0, CU::Matrix4x4<float>(), aAnimationToBlendWith->Frames, result);
	return result;
}

void FGE::Animation::CalculateTransforms(uint32_t aCurrentFrame, uint32_t aNextFrame, uint32_t aCurrentFrameBlend, uint32_t aNextFrameBlend,
	float aDelta, float aDeltaBlend, float aBlendAlpha, Skeleton& aSkeleton, uint32_t aIndex, const CU::Matrix4x4<float>& aParentTransform,
	const std::vector<Frame>& aFramesBlend, std::vector<CU::Matrix4x4<float>>& aTransforms) const
{
	const auto& bone = aSkeleton.Bones[aIndex];

	auto translation = Frames[aCurrentFrame].LocalTransforms[aIndex].Translation;
	auto rotation = Frames[aCurrentFrame].LocalTransforms[aIndex].Rotation;
	auto scale = Frames[aCurrentFrame].LocalTransforms[aIndex].Scale;
	{
		auto nextTranslation = Frames[aNextFrame].LocalTransforms[aIndex].Translation;

		auto nextRotation = Frames[aNextFrame].LocalTransforms[aIndex].Rotation;

		auto nextScale = Frames[aNextFrame].LocalTransforms[aIndex].Scale;

		translation = translation.Lerp(translation, nextTranslation, aDelta);
		rotation = rotation.Slerp(rotation, nextRotation, aDelta);
		scale = scale.Lerp(scale, nextScale, aDelta);
	}

	{
		auto translationBlend = aFramesBlend[aCurrentFrameBlend].LocalTransforms[aIndex].Translation;
		auto rotationBlend = aFramesBlend[aCurrentFrameBlend].LocalTransforms[aIndex].Rotation;
		auto scaleBlend = aFramesBlend[aCurrentFrameBlend].LocalTransforms[aIndex].Scale;

		auto nextTranslationBlend = aFramesBlend[aNextFrameBlend].LocalTransforms[aIndex].Translation;
		auto nextRotationBlend = aFramesBlend[aNextFrameBlend].LocalTransforms[aIndex].Rotation;
		auto nextScaleBlend = aFramesBlend[aNextFrameBlend].LocalTransforms[aIndex].Scale;

		translationBlend = translationBlend.Lerp(translationBlend, nextTranslationBlend, aDeltaBlend);
		rotationBlend = rotationBlend.Slerp(rotationBlend, nextRotationBlend, aDeltaBlend);
		scaleBlend = scaleBlend.Lerp(scaleBlend, nextScaleBlend, aDeltaBlend);

		translation = translation.Lerp(translation, translationBlend, aBlendAlpha);
		rotation = rotation.Slerp(rotation, rotationBlend, aBlendAlpha);
		scale = scale.Lerp(scale, scaleBlend, aBlendAlpha);

	}


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
		CalculateTransforms(aCurrentFrame, aNextFrame, aCurrentFrameBlend, aNextFrameBlend,
			aDelta, aDeltaBlend, aBlendAlpha, aSkeleton, bone.Children[i],
			boneTransform, aFramesBlend, aTransforms);
	}


}

float FGE::Animation::GetDuration() const 
{
	float secondsPerFrame = 1 / FramesPerSecond;
	return (FrameCount-1) * secondsPerFrame;
}
