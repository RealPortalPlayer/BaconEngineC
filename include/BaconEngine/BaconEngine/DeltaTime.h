// Purpose: Stores the current delta-time.
// Created on: 10/11/22 @ 9:28 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>

#include "BinaryExport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BE_BINARYEXPORT double BE_DeltaTime_GetMilliseconds(void);
BE_BINARYEXPORT double BE_DeltaTime_GetSeconds(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
