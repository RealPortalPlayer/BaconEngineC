// Purpose: Window functions only to be called by the engine's entry point.
// Created on: 9/13/22 @ 9:08 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>

#include "../AntiClientGuard.h"
#include "BaconEngine/Math/Vector.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
void BE_PrivateWindow_Initialize(const char* windowTitle, BE_Vector2_Unsigned windowSize);
void BE_PrivateWindow_Destroy(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
