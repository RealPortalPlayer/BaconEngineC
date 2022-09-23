// Purpose: Secret UI Window getter.
// Created on: 9/11/22 @ 7:42 PM

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

#include "BaconEngine/Storage/DynamicArray.h"
#include "BaconEngine/Rendering/UI.h"

SEC_CPP_SUPPORT_GUARD_START()
typedef struct {
    const char* name;
    BE_DynamicArray elements;
    BE_UI_WindowFlags flags;
    BE_Vector_2I position;
    BE_Vector_2U size;
    int currentRenderPosition;
    int windowId;
} BE_PrivateUI_Window;

const BE_DynamicArray* BE_PrivateUI_GetWindows(void);
const BE_DynamicArray* BE_PrivateUI_GetRenderWindows(void);
SEC_CPP_SUPPORT_GUARD_END()
