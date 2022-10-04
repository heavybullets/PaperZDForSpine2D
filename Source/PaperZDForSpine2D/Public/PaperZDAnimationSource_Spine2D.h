// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include "spine/spine.h"
#include "PaperZDAnimationSource_Spine2D.generated.h"

class USpineAtlasAsset;
class USpineSkeletonDataAsset;

/**
 * An animation source to be used with Spine2D animation components.
 */
UCLASS()
class PAPERZDFORSPINE2D_API UPaperZDAnimationSource_Spine2D : public UPaperZDAnimationSource
{
	GENERATED_BODY()

	/* The asset containing the texture information for rendering. */
	UPROPERTY(EditAnywhere, Category = "Animation")
	USpineAtlasAsset* Atlas;

	/* The asset containing the skeleton and animation data. */
	UPROPERTY(EditAnywhere, Category = "Animation")
	USpineSkeletonDataAsset* SkeletonDataAsset;

	/* If true, the skeleton data cache is valid. */
	UPROPERTY(Transient)
	mutable bool bSkeletonDataCacheValid;

	/* The cached skeleton data*/
	mutable spine::SkeletonData* CachedSkeletonData;

public:
	//ctor
	UPaperZDAnimationSource_Spine2D();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	//~ Begin UPaperZDAnimationSource Interface
	virtual TSubclassOf<UPaperZDPlaybackHandle> GetPlaybackHandleClass() const override;
	virtual TSubclassOf<UPrimitiveComponent> GetRenderComponentClass() const override;
	virtual void InitPlaybackHandle(UPaperZDPlaybackHandle* Handle) const override;
	//~ End UPaperZDAnimationSource Interface

	/* Obtain a list of all the available animation names. */
	void GetAnimationNames(TArray<FString>& OutNames) const;

	/* Obtain the underlying Spine2D animation object for the given animation name, or null if not found. */
	spine::Animation* FindAnimation(const FString& AnimationName) const;

	/**
	 * Obtain and cache the underlying Spine2D skeleton data.
	 * If the setup is not valid an error will be displayed (due to the Spine2D plugin) and the result will be cached to avoid subsequent errors getting triggered.
	 */
	spine::SkeletonData* GetCachedSkeletonData() const;

private:
	/* Invalidates the cached data. */
	void InvalidateCache();
};
