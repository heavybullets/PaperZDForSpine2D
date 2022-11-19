// Copyright 2017-2019 CriticalFailure Studio.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "PaperZDCharacter_Spine2D.generated.h"

class USpineSkeletonRendererComponent;
class USpineSkeletonAnimationComponent;

/**
 * Specialized character that manages Spine2D AnimSequence configuration
 */
UCLASS()
class PAPERZDFORSPINE2D_API APaperZDCharacter_Spine2D : public APaperZDCharacter
{
	GENERATED_BODY()

	/* Spine renderer component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spine2D", meta = (AllowPrivateAccess = "true"))
	USpineSkeletonRendererComponent* SpineRenderer;

	/* Skeleton animation component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spine2D", meta = (AllowPrivateAccess = "true"))
	USpineSkeletonAnimationComponent* SpineAnimationComponent;

public:
	APaperZDCharacter_Spine2D();
	   	
};
