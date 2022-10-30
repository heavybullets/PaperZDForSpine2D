// Copyright 2017-2019 CriticalFailure Studio.

#include "PaperZDCharacter_Spine2D.h"
#include "SpineSkeletonRendererComponent.h"
#include "SpineSkeletonAnimationComponent.h"
#include "PaperZDAnimSequence_Spine2D.h"
#include "AnimSequences/Players/PaperZDAnimPlayer.h"
#include "PaperZDAnimationComponent.h"

APaperZDCharacter_Spine2D::APaperZDCharacter_Spine2D() : Super()
{
	SpineRenderer = CreateDefaultSubobject<USpineSkeletonRendererComponent>(TEXT("SpineRenderer"));
	SpineRenderer->SetupAttachment(RootComponent);

	SpineAnimationComponent = CreateDefaultSubobject<USpineSkeletonAnimationComponent>(TEXT("SkeletonAnimation"));
	GetAnimationComponent()->InitRenderComponent(SpineRenderer);
}

