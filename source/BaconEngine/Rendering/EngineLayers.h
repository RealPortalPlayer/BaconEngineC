// Purpose: All the built-in engine layers.
// Created on: 8/9/22 @ 7:28 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "../AntiClientGuard.h"
#include "BaconEngine/Storage/DynamicArray.h"

SEC_CPP_SUPPORT_GUARD_START()
int BE_EngineLayers_GetUIWindowRenderCount(void);

void BE_EngineLayers_Initialize(void);
SEC_CPP_SUPPORT_GUARD_END()
