// Purpose: Window functions only to be called by the engine's entry point.
// Created on: 9/13/22 @ 9:08 PM

// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "BaconEngine/Math/Vector.h"

SEC_CPP_SUPPORT_GUARD_START()
void BE_PrivateWindow_Initialize(const char* windowTitle, BE_Vector_2U windowSize);
void BE_PrivateWindow_Destroy(void);
SEC_CPP_SUPPORT_GUARD_END()
