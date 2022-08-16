// Purpose: Every supported mouse related stuff.
// Created on: 8/15/22 @ 10:29 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

CPP_GUARD_START()
    typedef enum {
        BE_MOUSE_BUTTON_TYPE_LEFT = 1,
        BE_MOUSE_BUTTON_TYPE_MIDDLE,
        BE_MOUSE_BUTTON_TYPE_RIGHT,
        BE_MOUSE_BUTTON_TYPE_THIRD,
        BE_MOUSE_BUTTON_TYPE_FORTH
    } BE_MouseButtonType;
CPP_GUARD_END()
