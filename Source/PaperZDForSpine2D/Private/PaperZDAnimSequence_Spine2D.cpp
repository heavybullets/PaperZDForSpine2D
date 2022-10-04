// Copyright 2017-2022 CriticalFailure Studio.

#include "PaperZDAnimSequence_Spine2D.h"
#include "PaperZDAnimationSource_Spine2D.h"
#include "PaperZDForSpine2DCustomVersion.h"

void UPaperZDAnimSequence_Spine2D::PostLoad()
{
	Super::PostLoad();

	//Backwards compatibility: after the AnimBlueprint rework, the AnimSequences now can support "multi-directional" data sources
	//which are stored on an array that accommodates its size to the number of directions it needs to use.
	//A non-directional sequence uses a size-1 array, thus we need to move the data from the old non-directional variable into the array.
	//The resulting AnimSequence will be non-directional due to the old version having no directional support whatsoever.
	const int32 ZDVersion = GetLinkerCustomVersion(FPaperZDForSpine2DCustomVersion::GUID);
	if (ZDVersion < FPaperZDForSpine2DCustomVersion::AddedDirectionalSequenceSupport)
	{
		AnimDataSource.SetNum(1);
		AnimDataSource[0] = AnimationName_DEPRECATED.ToString();
		bDirectionalSequence = false;
	}
}

void UPaperZDAnimSequence_Spine2D::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
	Ar.UsingCustomVersion(FPaperZDForSpine2DCustomVersion::GUID);
}

float UPaperZDAnimSequence_Spine2D::GetTotalDuration() const
{
	//Animation data available inside the source
	UPaperZDAnimationSource_Spine2D* SpineSource = Cast<UPaperZDAnimationSource_Spine2D>(GetAnimSource());
	if (SpineSource && AnimDataSource.Num())
	{
		//Try to find this animation, if it exists (edge cases could be that we changed the source data or haven't setup the data correctly)
		const FString& PrimaryAnimationName = AnimDataSource[0];
		spine::Animation* AnimationData = SpineSource->FindAnimation(PrimaryAnimationName);
		if (AnimationData)
		{
			return AnimationData->getDuration();
		}
	}

	return 0.0f;
}

float UPaperZDAnimSequence_Spine2D::GetFramesPerSecond() const
{
	//Default value is 15 fps
	return 15.0f;
}

bool UPaperZDAnimSequence_Spine2D::IsDataSourceEntrySet(int32 EntryIndex) const
{
	return AnimDataSource.IsValidIndex(EntryIndex) ? !AnimDataSource[EntryIndex].IsEmpty() : false;
}

TArray<FString> UPaperZDAnimSequence_Spine2D::GetAvailableAnimationNames() const
{
	TArray<FString> AnimationNames;

	//Animation data available inside the source
	UPaperZDAnimationSource_Spine2D* SpineSource = Cast<UPaperZDAnimationSource_Spine2D>(GetAnimSource());
	if (SpineSource)
	{
		SpineSource->GetAnimationNames(AnimationNames);
	}

	return AnimationNames;
}
