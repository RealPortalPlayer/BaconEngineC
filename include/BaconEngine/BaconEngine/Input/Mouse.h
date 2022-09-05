// Purpose: Every supported mouse related stuff.
// Created on: 8/15/22 @ 10:29 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

SEC_CPP_GUARD_START()
    typedef enum {
        BE_MOUSE_BUTTON_TYPE_LEFT = 1,
        BE_MOUSE_BUTTON_TYPE_MIDDLE,
        BE_MOUSE_BUTTON_TYPE_RIGHT,
        BE_MOUSE_BUTTON_TYPE_THIRD,
        BE_MOUSE_BUTTON_TYPE_FORTH
    } BE_Mouse_ButtonType;

    // TODO: BE_Mouse_IsDown, BE_Mouse_IsUp, BE_Mouse_IsPressed, BE_Mouse_WasDown
    //       Frame 1:  1                0              1                   0
    //       Frame 2:  0                0              0                   1
    //       Frame 3:  0                1              0                   0
    //       Frame 4:  1                0              1                   0
    //       Frame 5:  0                0              1                   0
    //       Frame 6:  0                0              1                   0
    //       Frame 7:  1                0              1                   0
    //       Frame 8:  1                0              1                   0
    //       Frame 9:  0                0              0                   1
    //       Frame 10: 0                1              0                   0
SEC_CPP_GUARD_END()
