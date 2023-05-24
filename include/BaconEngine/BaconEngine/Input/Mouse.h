// Purpose: Every supported mouse related stuff.
// Created on: 8/15/22 @ 10:29 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>
#include <SharedEngineCode/Internal/Boolean.h>

#include "BaconEngine/BinaryExport.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
typedef enum {
    BE_MOUSE_BUTTON_TYPE_FIRST = 1,
    BE_MOUSE_BUTTON_TYPE_SECOND,
    BE_MOUSE_BUTTON_TYPE_THIRD,
    BE_MOUSE_BUTTON_TYPE_FOURTH,
    BE_MOUSE_BUTTON_TYPE_FIFTH,

    BE_MOUSE_BUTTON_TYPE_LEFT = BE_MOUSE_BUTTON_TYPE_FIRST,
    BE_MOUSE_BUTTON_TYPE_MIDDLE = BE_MOUSE_BUTTON_TYPE_THIRD,
    BE_MOUSE_BUTTON_TYPE_RIGHT = BE_MOUSE_BUTTON_TYPE_SECOND
} BE_Mouse_ButtonType;

BE_BINARYEXPORT SEC_Boolean BE_Mouse_IsButtonDown(BE_Mouse_ButtonType button);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
