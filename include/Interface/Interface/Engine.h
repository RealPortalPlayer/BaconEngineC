// Purpose: Manages the engine binary
// Created on: 4/22/2023 @ 2:01 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/PlatformSpecific.h>

BA_CPLUSPLUS_SUPPORT_GUARD_START()
void* I_Engine_GetBinary(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()

#define I_ENGINE_FUNCTION_EXISTS(functionName) BA_PLATFORMSPECIFIC_GET_ADDRESS(I_Engine_GetBinary(), functionName) != NULL
