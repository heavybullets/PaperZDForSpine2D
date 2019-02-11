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
		SkeletonData = SkeletonDataAsset->GetSkeletonData(Atlas->GetAtlas());

		if (SkeletonData && !AnimationName.IsNone())
		{
			CachedAnimation = SkeletonData->findAnimation(TCHAR_TO_UTF8(*AnimationName.ToString()));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//// Required sequence methods
//////////////////////////////////////////////////////////////////////////
void UPaperZDAnimSequence_Spine2D::BeginSequencePlayback(class UPrimitiveComponent* RenderComponent, bool bLooping, bool bIsPreviewPlayback /* = false */) const
{
	USpineSkeletonRendererComponent* SpineRender = Cast<USpineSkeletonRendererComponent>(RenderComponent);
	if (SpineRender && CachedAnimation)
	{
		AActor* Owner = RenderComponent->GetOwner();
		USpineSkeletonAnimationComponent* SkeletonComponent = Owner ? Owner->FindComponentByClass<USpineSkeletonAnimationComponent>() : AnimationComponent;
		if (SkeletonComponent)
		{
			SkeletonComponent->Atlas = Atlas;
			SkeletonComponent->SkeletonData = SkeletonDataAsset;
			SkeletonComponent->AddAnimation(0, AnimationName, bLooping, 0.0f);

			//Preview scene extra steps
			if (bIsPreviewPlayback)
			{
				SkeletonComponent->SetAutoPlay(false);
				SkeletonComponent->SetPlaybackTime(Time);
			}
			else
			{
				SkeletonComponent->SetAutoPlay(true);
			}
		}
	}
}

void UPaperZDAnimSequence_Spine2D::UpdateRenderPlayback(class UPrimitiveComponent* RenderComponent, const float Time, bool bLooping, bool bIsPreviewPlayback /* = false */) const
{
	USpineSkeletonRendererComponent* SpineRender = Cast<USpineSkeletonRendererComponent>(RenderComponent);
	if (SpineRender && CachedAnimation)
	{
		AActor* Owner = RenderComponent->GetOwner();
		USpineSkeletonAnimationComponent* SkeletonComponent = Owner ? Owner->FindComponentByClass<USpineSkeletonAnimationComponent>() : AnimationComponent;
		if (SkeletonComponent)
		{
			SkeletonComponent->Atlas = Atlas;
			SkeletonComponent->SkeletonData = SkeletonDataAsset;

			//Check that the tracks are correctly configured
			/*spine::AnimationState* State = SkeletonComponent->GetAnimationState();
			if (State)
			{
				if (State->getCurrent(0) == nullptr || State->getCurrent(0)->getAnimation()->getName() != CachedAnimation->getName())
				{
					if (State->getCurrent(0))
					{
						const FString str1 = FString(UTF8_TO_TCHAR(State->getCurrent(0)->getAnimation()->getName().buffer()));;
						const FString str2 = FString(UTF8_TO_TCHAR(CachedAnimation->getName().buffer()));;
					}
					
					//State->clearTracks();
					State->addAnimation(0, CachedAnimation, true, 0.0f);
				}
			}*/

			//Preview scene extra steps
			if (bIsPreviewPlayback)
			{
				SpineRender->UpdateRenderer(SkeletonComponent);
				SkeletonComponent->SetPlaybackTime(Time);
			}
		}
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

void UPaperZDAnimSequence_Spine2D::ConfigureRenderComponent(class UPrimitiveComponent* RenderComponent, bool bIsPreviewPlayback /* = false */) const
{
// 	USpineSkeletonRendererComponent* SpineRender = Cast<USpineSkeletonRendererComponent>(RenderComponent);
// 	if (SpineRender)
// 	{
// 		if (bIsPreviewPlayback)
// 		{
// 			USpineSkeletonAnimationComponent* CreatedAnimationComponent = Owner->FindComponentByClass<USpineSkeletonAnimationComponent>();
// 
// 			//We must make sure we have an AnimationComponent for Spine2D
// 			if (!CreatedAnimationComponent)
// 			{
// 				CreatedAnimationComponent = NewObject<USpineSkeletonAnimationComponent>(Owner);
// 				Owner->AddOwnedComponent(CreatedAnimationComponent);
// 			}
// 
// 			/* Runtime Playback is managed by Spine2D. */
// 			CreatedAnimationComponent->SetAutoPlay(true);
// 		}
// #if WITH_EDITOR
// 		else
// 		{
// 			//No owner, means we're on a preview scene.
// 			
// 		}
// #endif
// 	}
}