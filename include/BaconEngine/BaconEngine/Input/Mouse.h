// Purpose: Every supported mouse related stuff.
// Created on: 8/15/22 @ 10:29 PM

// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CppSupport.h>

SEC_CPP_SUPPORT_GUARD_START()
typedef enum {
    BE_MOUSE_BUTTON_TYPE_LEFT = 1,
    BE_MOUSE_BUTTON_TYPE_MIDDLE,
    BE_MOUSE_BUTTON_TYPE_RIGHT,
    BE_MOUSE_BUTTON_TYPE_THIRD,
    BE_MOUSE_BUTTON_TYPE_FORTH
} BE_Mouse_ButtonType;

void BE_Mouse_SetButtonDown(BE_Mouse_ButtonType button, int down);
int BE_Mouse_IsButtonDown(BE_Mouse_ButtonType button);
SEC_CPP_SUPPORT_GUARD_END()
