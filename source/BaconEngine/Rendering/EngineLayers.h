// Purpose: All the built-in engine layers.
// Created on: 8/9/22 @ 7:28 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Storage/DynamicArray.h>

#include "../AntiClientGuard.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
int BE_EngineLayers_GetUIWindowRenderCount(void);

void BE_EngineLayers_Initialize(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
