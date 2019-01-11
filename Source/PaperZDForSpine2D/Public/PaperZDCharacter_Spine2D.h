// Copyright 2017-2019 CriticalFailure Studio.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "PaperZDCharacter_Spine2D.generated.h"

class USpineSkeletonRendererComponent;
/**
 * Specialized character that manages Spine2D AnimSequence configuration
 */
UCLASS()
class PAPERZDFORSPINE2D_API APaperZDCharacter_Spine2D : public APaperZDCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Spine2D")
	USpineSkeletonRendererComponent* SpineRenderer;

public:
	APaperZDCharacter_Spine2D();

	//Required methods for initialization
	virtual void ConfigurePlayer_Implementation(UPaperZDAnimPlayer* Player);
	   	
};
