// Copyright 2017-2019 CriticalFailure Studio.

#include "PaperZDAnimSequence_Spine2D.h"
#include "SpineSkeletonRendererComponent.h"
#include "SpineSkeletonAnimationComponent.h"
#include "SpineAtlasAsset.h"

UPaperZDAnimSequence_Spine2D::UPaperZDAnimSequence_Spine2D() : Super()
{
#if WITH_EDITOR
	//Preview scene needed AnimationComponent
	AnimationComponent = CreateDefaultSubobject<USpineSkeletonAnimationComponent>(TEXT("PreviewAnimationComponent"), true);
	AnimationComponent->SetAutoPlay(false);
#endif
}

void UPaperZDAnimSequence_Spine2D::PostLoad()
{
	Super::PostLoad();

	if (Atlas && SkeletonDataAsset)
	{
		UpdateAnimationData();
	}
}

#if WITH_EDITOR
void UPaperZDAnimSequence_Spine2D::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == TEXT("Atlas") || PropertyChangedEvent.GetPropertyName() == TEXT("SkeletonDataAsset"))
	{
		UpdateAnimationData();
	}
	else if (PropertyChangedEvent.GetPropertyName() == TEXT("AnimationName"))
	{
		//No need to update the skeleton if only the animation changes
		if (SkeletonData)
		{
			CachedAnimation = SkeletonData->findAnimation(TCHAR_TO_UTF8(*AnimationName.ToString()));
		}
	}
}
#endif

void UPaperZDAnimSequence_Spine2D::UpdateAnimationData()
{
	//Regenerate data
	if (Atlas && SkeletonDataAsset)
	{
		SkeletonData = SkeletonDataAsset->GetSkeletonData(Atlas->GetAtlas(false), false);

		if (SkeletonData && !AnimationName.IsNone())
		{
			CachedAnimation = SkeletonData->findAnimation(TCHAR_TO_UTF8(*AnimationName.ToString()));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//// Required sequence methods
//////////////////////////////////////////////////////////////////////////
void UPaperZDAnimSequence_Spine2D::UpdateRenderPlayback(class UPrimitiveComponent* RenderComponent, const float Time) const
{
	USpineSkeletonRendererComponent* SpineRender = Cast<USpineSkeletonRendererComponent>(RenderComponent);
	if (SpineRender)
	{
		USpineSkeletonAnimationComponent* SkeletonComponent = Cast<USpineSkeletonAnimationComponent>(SpineRender->GetSkeleton());
		if (SkeletonComponent)
		{
			SkeletonComponent->Atlas = Atlas;
			SkeletonComponent->SkeletonData = SkeletonDataAsset;

			//Check that the tracks are correctly configured
			spine::AnimationState* State = SkeletonComponent->GetAnimationState();
			if (State)
			{
				if (State->getCurrent(0) == nullptr || State->getCurrent(0)->getAnimation()->getName() != CachedAnimation->getName())
				{
					State->clearTracks();
					State->addAnimation(0, CachedAnimation, false, 0.0f);
				}
			}

			//Change time
			SkeletonComponent->SetPlaybackTime(Time);
		}

#if WITH_EDITOR
		//Preview scene extra steps
		if (!SpineRender->GetOwner())
		{
			SpineRender->UpdateRenderer();
		}
#endif
	}
}

float UPaperZDAnimSequence_Spine2D::GetTotalDuration() const
{
	return CachedAnimation ? CachedAnimation->getDuration() : 0.0f;
}

TSubclassOf<UPrimitiveComponent> UPaperZDAnimSequence_Spine2D::GetRenderComponentClass() const
{
	return USpineSkeletonRendererComponent::StaticClass();
}

void UPaperZDAnimSequence_Spine2D::ConfigureRenderComponent(UPrimitiveComponent* RenderComponent) const
{
	USpineSkeletonRendererComponent* SpineRender = Cast<USpineSkeletonRendererComponent>(RenderComponent);
	if (SpineRender)
	{
		AActor* Owner = RenderComponent->GetOwner();
		if (Owner)
		{
			USpineSkeletonAnimationComponent* CreatedAnimationComponent = Owner->FindComponentByClass<USpineSkeletonAnimationComponent>();

			//We must make sure we have an AnimationComponent for Spine2D
			if (!CreatedAnimationComponent)
			{
				CreatedAnimationComponent = NewObject<USpineSkeletonAnimationComponent>(Owner);
				Owner->AddOwnedComponent(CreatedAnimationComponent);
				SpineRender->SetSkeleton(CreatedAnimationComponent);
			}

			/* Playback should be paused so we manually manage it. */
			CreatedAnimationComponent->SetAutoPlay(false);
		}
#if WITH_EDITOR
		else
		{
			//No owner, means we're on a preview scene.
			SpineRender->SetSkeleton(AnimationComponent);
		}
#endif
	}
}