// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "PaperZDAnimationSource_Spine2D.h"
#include "PaperZDPlaybackHandle_Spine2D.h"
#include "PaperZDAnimSequence_Spine2D.h"
#include "SpineSkeletonRendererComponent.h"
#include "SpineAtlasAsset.h"

UPaperZDAnimationSource_Spine2D::UPaperZDAnimationSource_Spine2D()
{
	SupportedAnimSequenceClass = UPaperZDAnimSequence_Spine2D::StaticClass();
	bSupportsBlending = false;
	bSupportsAnimationLayers = true;
	bSkeletonDataCacheValid = false;
	CachedSkeletonData = nullptr;
}

#if WITH_EDITOR
void UPaperZDAnimationSource_Spine2D::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//If any of these values change, the skeleton data is no longer valid
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UPaperZDAnimationSource_Spine2D, Atlas)
		|| PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UPaperZDAnimationSource_Spine2D, SkeletonDataAsset))
	{
		InvalidateCache();
	}
}
#endif

TSubclassOf<UPaperZDPlaybackHandle> UPaperZDAnimationSource_Spine2D::GetPlaybackHandleClass() const
{
	return UPaperZDPlaybackHandle_Spine2D::StaticClass();
}

TSubclassOf<UPrimitiveComponent> UPaperZDAnimationSource_Spine2D::GetRenderComponentClass() const
{
	return USpineSkeletonRendererComponent::StaticClass();
}

void UPaperZDAnimationSource_Spine2D::InitPlaybackHandle(UPaperZDPlaybackHandle* Handle) const
{
	UPaperZDPlaybackHandle_Spine2D* SpineHandle = Cast<UPaperZDPlaybackHandle_Spine2D>(Handle);
	if (SpineHandle)
	{
		SpineHandle->InitRenderData(Atlas, SkeletonDataAsset);
	}
}

void UPaperZDAnimationSource_Spine2D::GetAnimationNames(TArray<FString>& OutNames) const
{
	if (SkeletonDataAsset)
	{
		OutNames = SkeletonDataAsset->Animations;
	}
}

spine::Animation* UPaperZDAnimationSource_Spine2D::FindAnimation(const FString& AnimationName) const
{
	if (SkeletonDataAsset && Atlas)
	{
		spine::SkeletonData* SkeletonData = GetCachedSkeletonData();
		if (SkeletonData)
		{
			return SkeletonData->findAnimation(TCHAR_TO_UTF8(*AnimationName));
		}
	}

	return nullptr;
}

spine::SkeletonData* UPaperZDAnimationSource_Spine2D::GetCachedSkeletonData() const
{
	if (!bSkeletonDataCacheValid && SkeletonDataAsset && Atlas)
	{
		CachedSkeletonData = SkeletonDataAsset->GetSkeletonData(Atlas->GetAtlas());
		bSkeletonDataCacheValid = true;
	}

	return CachedSkeletonData;
}

void UPaperZDAnimationSource_Spine2D::InvalidateCache()
{
	bSkeletonDataCacheValid = false;
	CachedSkeletonData = nullptr;
}
