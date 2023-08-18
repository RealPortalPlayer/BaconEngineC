// Purpose: All the built-in engine layers.
// Created on: 8/9/22 @ 7:28 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>
#include <SharedEngineCode/Storage/DynamicArray.h>

#include "../AntiClientGuard.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
int BE_EngineLayers_GetUIWindowRenderCount(void);

void BE_EngineLayers_Initialize(void);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
