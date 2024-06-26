// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "PaperZDPlaybackHandle_Spine2D.h"
#include "AnimSequences/Players/PaperZDAnimationPlaybackData.h"
#include "SpineSkeletonRendererComponent.h"
#include "SpineSkeletonAnimationComponent.h"
#include "spine/Version.h"
#if SPINE_MAJOR_VERSION >= 4 && SPINE_MINOR_VERSION >= 2
#include "spine/Physics.h"
#endif

void UPaperZDPlaybackHandle_Spine2D::UpdateRenderPlayback(UPrimitiveComponent* RenderComponent, const FPaperZDAnimationPlaybackData& PlaybackData, bool bIsPreviewPlayback /* = false */)
{
	USpineSkeletonRendererComponent* SpineRender = Cast<USpineSkeletonRendererComponent>(RenderComponent);
	if (SpineRender && AnimationComponent)
	{
		//Iterate on all the animations, layering them on the tracks
		for (const FPaperZDWeightedAnimation& WeightedAnimation : PlaybackData.WeightedAnimations)
		{
			//We will work directly with the tracks, so we can manage the playback times directly
			const FString& AnimationName = WeightedAnimation.AnimSequencePtr->GetAnimationData<FString>(PlaybackData.DirectionalAngle, bIsPreviewPlayback);
			UTrackEntry* TrackEntry = AnimationComponent->GetCurrent(WeightedAnimation.Layer);
			if (!TrackEntry || TrackEntry->getAnimationName() != AnimationName)
			{
				TrackEntry = AnimationComponent->SetAnimation(WeightedAnimation.Layer, AnimationName, false);
				TrackEntry->SetAlpha(WeightedAnimation.LayerWeight);
			}

			//@TODO: Set Timescale? maybe access to specific logic inside the blueprint (like playback rate) to get that
			//		adding a "NodeId" value to the playback would enable us to query the node that played it, so we can check against its values
		}
		
		//At this point, the animation state should have been created (but can still be null if the animation data is invalid)
		if (AnimationComponent->GetAnimationState())
		{
			//@Note:
			// We now allow a dummy update so the mixing time logic moves forward 
			// 
			// This uses the world delta seconds, which means mixing could be wrong.
			// For fixing this behavior, we need to be able to grab the last delta time used by the AnimInstance.
			// which isn't available easily.
			const float DeltaTime = RenderComponent->GetWorld()->GetDeltaSeconds();
			AnimationComponent->GetAnimationState()->update(DeltaTime);

			//Because the animations don't necessarily update at the same speed as the tick time (due to playback speed), we need to setup the animations
			for (const FPaperZDWeightedAnimation& WeightedAnimation : PlaybackData.WeightedAnimations)
			{
				UTrackEntry* TrackEntry = AnimationComponent->GetCurrent(WeightedAnimation.Layer);
				TrackEntry->SetTrackTime(WeightedAnimation.PlaybackTime);
				TrackEntry->SetAlpha(WeightedAnimation.LayerWeight);
			}

			//Update the skeleton
			spine::Skeleton* Skeleton = AnimationComponent->GetSkeleton();
			Skeleton->update(DeltaTime);
			AnimationComponent->GetAnimationState()->apply(*Skeleton);

#if SPINE_MAJOR_VERSION >= 4 && SPINE_MINOR_VERSION >= 2
			AnimationComponent->BeforeUpdateWorldTransform.Broadcast(AnimationComponent);
			Skeleton->updateWorldTransform(spine::Physics::Physics_Update);
			AnimationComponent->AfterUpdateWorldTransform.Broadcast(AnimationComponent);
#elif 
			Skeleton->updateWorldTransform();
#endif

			//Finally update the renderer
			SpineRender->UpdateRenderer(AnimationComponent);
		}
	}
}

void UPaperZDPlaybackHandle_Spine2D::ConfigureRenderComponent(UPrimitiveComponent* RenderComponent, bool bIsPreviewPlayback /* = false */)
{
	USpineSkeletonRendererComponent* SpineRender = Cast<USpineSkeletonRendererComponent>(RenderComponent);
	if (SpineRender)
	{
		AActor* Owner = RenderComponent->GetOwner();
		AnimationComponent = Owner ? Owner->FindComponentByClass<USpineSkeletonAnimationComponent>() : nullptr;

		//If no animation component was found, we create one
		if (!AnimationComponent)
		{
			AnimationComponent = NewObject<USpineSkeletonAnimationComponent>();
		}

		//Now setup data
		if (AnimationComponent)
		{
			AnimationComponent->Atlas = Atlas;
			AnimationComponent->SkeletonData = SkeletonDataAsset;
			AnimationComponent->SetAutoPlay(false);
		}
	}
}

void UPaperZDPlaybackHandle_Spine2D::InitRenderData(USpineAtlasAsset* InAtlas, USpineSkeletonDataAsset* InSkeletonDataAsset)
{
	Atlas = InAtlas;
	SkeletonDataAsset = InSkeletonDataAsset;
}
