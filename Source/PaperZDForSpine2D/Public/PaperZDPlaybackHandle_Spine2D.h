// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimSequences/Players/PaperZDPlaybackHandle.h"
#include "PaperZDPlaybackHandle_Spine2D.generated.h"

class USpineAtlasAsset;
class USpineSkeletonDataAsset;
class USpineSkeletonAnimationComponent;

/**
 * Playback handle that manages rendering of Spine2D render components.
 */
UCLASS()
class PAPERZDFORSPINE2D_API UPaperZDPlaybackHandle_Spine2D : public UPaperZDPlaybackHandle
{
	GENERATED_BODY()

	/* Pointer to the spine atlas used for playback setup. */
	UPROPERTY(Transient)
	USpineAtlasAsset* Atlas;

	/* Skeleton data asset that contains the bone information. */
	UPROPERTY(Transient)
	USpineSkeletonDataAsset* SkeletonDataAsset;

	/* Cached spine animation component which is used to update the renderer. */
	UPROPERTY(Transient)
	USpineSkeletonAnimationComponent* AnimationComponent;
	
public:
	//~ Begin UPaperZDPlaybackHandle Interface
	virtual void UpdateRenderPlayback(UPrimitiveComponent* RenderComponent, const FPaperZDAnimationPlaybackData& PlaybackData, bool bIsPreviewPlayback = false) override;
	virtual void ConfigureRenderComponent(UPrimitiveComponent* RenderComponent, bool bIsPreviewPlayback = false) override;
	//~ End UPaperZDPlaybackHandle Interface

	/* Setup the required data for spine 2d rendering. */
	void InitRenderData(USpineAtlasAsset* InAtlas, USpineSkeletonDataAsset* InSkeletonDataAsset);

private:
	
};
