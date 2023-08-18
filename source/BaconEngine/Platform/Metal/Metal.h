// Purpose: Metal entry point.
// Created on: 1/1/2023 @ 1:36 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>

#include "../../AntiClientGuard.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
void BE_Metal_Initialize(void);
void BE_Metal_Destroy(void);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
