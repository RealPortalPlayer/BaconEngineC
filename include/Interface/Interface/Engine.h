// Purpose: Manages the engine binary
// Created on: 4/22/2023 @ 2:01 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>
#include <SharedEngineCode/Internal/PlatformSpecific.h>

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
void* I_Engine_GetBinary(void);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()

#define I_ENGINE_FUNCTION_EXISTS(functionName) SEC_PLATFORMSPECIFIC_GET_ADDRESS(I_Engine_GetBinary(), functionName) != NULL
