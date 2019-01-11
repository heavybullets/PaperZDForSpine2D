// Copyright 2017-2019 CriticalFailure Studio.

#pragma once

#include "CoreMinimal.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "spine/spine.h"
#include "PaperZDAnimSequence_Spine2D.generated.h"

class USpineAtlasAsset;
class USpineSkeletonDataAsset;
class USpineSkeletonAnimationComponent;
/**
 * Specialized AnimSequence class that gives Spine2D support to PaperZD.
 */
UCLASS()
class PAPERZDFORSPINE2D_API UPaperZDAnimSequence_Spine2D : public UPaperZDAnimSequence
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Animation")
	USpineAtlasAsset* Atlas;

	UPROPERTY(EditAnywhere, Category = "Animation")
	USpineSkeletonDataAsset* SkeletonDataAsset;

	UPROPERTY(EditAnywhere, Category = "Animation")
	FName AnimationName;

	/* Holds Spine2D skeletal animation data */
	spine::SkeletonData* SkeletonData;

	/* Holds the cached Animation data from the skeleton, is managed by the skeleton */
	spine::Animation* CachedAnimation;

#if WITH_EDITOR
	/* Used for Preview scene viewport updating, as the Renderer won't have an owner nor an AnimationComponent, we must provide the latter. */
	UPROPERTY(Transient)
	USpineSkeletonAnimationComponent* AnimationComponent;
#endif

public:
	UPaperZDAnimSequence_Spine2D();

	// Needed for animation data initialization
	void PostLoad() override;
	
	//Required methods
	virtual void UpdateRenderPlayback(class UPrimitiveComponent* RenderComponent, const float Time) const override;
	virtual float GetTotalDuration() const override;
	virtual TSubclassOf<UPrimitiveComponent> GetRenderComponentClass() const override;
	virtual void ConfigureRenderComponent(class UPrimitiveComponent* RenderComponent) const override;
	
#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	void UpdateAnimationData();
	
};
