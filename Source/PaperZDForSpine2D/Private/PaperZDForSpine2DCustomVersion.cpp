// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "PaperZDForSpine2DCustomVersion.h"
#include "Serialization/CustomVersion.h"

const FGuid FPaperZDForSpine2DCustomVersion::GUID(0x11311AFD, 0x2E554D61, 0xAF679AA3, 0xC5A1082A);

// Register the custom version with core
FCustomVersionRegistration GRegisterPaperZDForSpine2DCustomVersion(FPaperZDForSpine2DCustomVersion::GUID, FPaperZDForSpine2DCustomVersion::LatestVersion, TEXT("Paper_ZD_Spine2D_Connector_Version"));