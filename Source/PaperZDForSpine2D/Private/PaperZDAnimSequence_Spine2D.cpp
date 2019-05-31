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
			SkeletonComponent->AddAnimation(0, AnimationName.ToString(), bLooping, 0.0f);
			SkeletonComponent->SetAutoPlay(!bIsPreviewPlayback);
		}
	}
}

void UPaperZDAnimSequence_Spine2D::UpdateRenderPlayback(class UPrimitiveComponent* RenderComponent, const float Time, bool bIsPreviewPlayback /* = false */) const
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

			//Preview playback has some extra steps
			if (bIsPreviewPlayback)
			{
#if WITH_EDITOR
				//When on editor, we can change the animation data after being inited, we must be able to respond to those changes which don't happen on runtime
				spine::AnimationState* State = SkeletonComponent->GetAnimationState();
				if (State)
				{
					if (State->getCurrent(0) == nullptr || State->getCurrent(0)->getAnimation()->getName() != CachedAnimation->getName())
					{
						if (State->getCurrent(0))
						{
							const FString str1 = FString(UTF8_TO_TCHAR(State->getCurrent(0)->getAnimation()->getName().buffer()));;
							const FString str2 = FString(UTF8_TO_TCHAR(CachedAnimation->getName().buffer()));;
						}

						State->clearTracks();
						State->addAnimation(0, CachedAnimation, true, 0.0f);
					}
				}
#endif

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
	//Extra initialization global to the AnimSequence goes here
}