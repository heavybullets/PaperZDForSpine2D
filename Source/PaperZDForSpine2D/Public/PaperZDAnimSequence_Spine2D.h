// Copyright 2017-2022 CriticalFailure Studio.

#pragma once

#include "CoreMinimal.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "PaperZDAnimSequence_Spine2D.generated.h"

/**
 * Specialized AnimSequence class that gives Spine2D support to PaperZD.
 */
UCLASS()
class PAPERZDFORSPINE2D_API UPaperZDAnimSequence_Spine2D : public UPaperZDAnimSequence
{
	GENERATED_BODY()

	//@DEPRECATED: In favor of the animation data source which now can support multi-directional animations.
	UPROPERTY()
	FName AnimationName_DEPRECATED;

	/* Contains the render information for displaying the animation, multi-directional. */
	UPROPERTY(EditAnywhere, Category = "AnimSequence", meta = (GetOptions = "GetAvailableAnimationNames"))
	TArray<FString> AnimDataSource;
		
public:

	//~ Begin UObject Interface
	virtual void PostLoad() override;
	virtual void Serialize(FArchive& Ar) override;
	//~ End UObject Interface

	//~ Begin UPaperZDAnimSequence Interface
	virtual float GetTotalDuration() const override;
	virtual float GetFramesPerSecond() const override;
	virtual bool IsDataSourceEntrySet(int32 EntryIndex) const override;
	//~ End UPaperZDAnimSequence Interface

private:
	/* Obtain the available animation names stored on the animation source. */
	UFUNCTION()
	TArray<FString> GetAvailableAnimationNames() const;
};
