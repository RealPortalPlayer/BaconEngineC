// Purpose: Secret UI Window getter.
// Created on: 9/11/22 @ 7:42 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>

#include "../AntiClientGuard.h"
#include "BaconEngine/Storage/DynamicArray.h"
#include "BaconEngine/Rendering/UI.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    const char* name;
    BE_DynamicArray elements;
    BE_UI_WindowFlags flags;
    BE_Vector2_Integer position;
    BE_Vector2_Unsigned size;
    int currentRenderPosition;
    int windowId;
} BE_PrivateUI_Window;

const BE_DynamicArray* BE_PrivateUI_GetWindows(void);
const BE_DynamicArray* BE_PrivateUI_GetRenderWindows(void);

void BE_PrivateUI_Initialize(void);
void BE_PrivateUI_Destroy(void);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
