// Purpose: An event.
// Created on: 8/15/22 @ 4:57 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>

#include "BaconEngine/Math/Vector.h"
#include "BaconEngine/Input/Keyboard.h"
#include "BaconEngine/Input/Mouse.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
typedef enum {
    BE_EVENT_TYPE_KEYBOARD_KEY_DOWN,
    BE_EVENT_TYPE_KEYBOARD_KEY_UP,
    BE_EVENT_TYPE_MOUSE_MOVED,
    BE_EVENT_TYPE_MOUSE_BUTTON_DOWN,
    BE_EVENT_TYPE_MOUSE_BUTTON_UP,
    BE_EVENT_TYPE_MOUSE_WHEEL,
    BE_EVENT_TYPE_MOUSE_ENTER,
    BE_EVENT_TYPE_MOUSE_LEAVE,
    BE_EVENT_TYPE_WINDOW_MOVED,
    BE_EVENT_TYPE_WINDOW_RESIZED,
    BE_EVENT_TYPE_WINDOW_RESTORED,
    BE_EVENT_TYPE_WINDOW_MAXIMIZED,
    BE_EVENT_TYPE_WINDOW_FOCUS_GAINED,
    BE_EVENT_TYPE_WINDOW_FOCUS_LOST,
    BE_EVENT_TYPE_WINDOW_CLOSE
} BE_Event_Type;

typedef struct {
    BE_Event_Type type;
    union {
        struct {
            union {
                BE_Vector2_Integer newPosition;
                BE_Vector2_Integer newSize;
            } unionVariables;
        } window;
        struct {
            BE_Keyboard_KeyCodes key;
            unsigned repeat;
        } keyboard;
        struct {
            BE_Vector2_Float position;
            union {
                BE_Mouse_ButtonType whichButton;
                BE_Vector2_Float wheelScrollAmount;
            } unionVariables;
        } mouse;
    } data;
} BE_Event;
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
