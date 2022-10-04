// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Guid.h"

// Custom serialization version for the PaperZD to Spine2D connector plugin
struct FPaperZDForSpine2DCustomVersion
{
	enum Type
	{
		// Before any version changes were made in the plugin
		BeforeCustomVersionWasAdded = 0,

		// Added AnimSequences, Sequencer Support and Animation Queue
		AddedDirectionalSequenceSupport = 1,

		// -----<new versions can be added above this line>-------------------------------------------------
		VersionPlusOne,
		LatestVersion = VersionPlusOne - 1
	};

	// The GUID for this custom version number
	const static FGuid GUID;

private:
	FPaperZDForSpine2DCustomVersion() {}
};
