// Copyright 2017-2019 CriticalFailure Studio.

#include "PaperZDCharacter_Spine2D.h"
#include "SpineSkeletonRendererComponent.h"
#include "SpineSkeletonAnimationComponent.h"
#include "PaperZDAnimSequence_Spine2D.h"
#include "AnimSequences/Players/PaperZDAnimPlayer.h"

APaperZDCharacter_Spine2D::APaperZDCharacter_Spine2D() : Super()
{
	SpineRenderer = CreateDefaultSubobject<USpineSkeletonRendererComponent>(TEXT("SpineRenderer"));
	SpineRenderer->SetupAttachment(RootComponent);

	AnimationComponent = CreateDefaultSubobject<USpineSkeletonAnimationComponent>(TEXT("SkeletonAnimation"));
}

void APaperZDCharacter_Spine2D::ConfigurePlayer_Implementation(UPaperZDAnimPlayer* Player)
{
	Super::ConfigurePlayer_Implementation(Player);

	//Need to register the sequence with our intended spine renderer
	Player->RegisterRenderComponent(UPaperZDAnimSequence_Spine2D::StaticClass(), SpineRenderer);
}

