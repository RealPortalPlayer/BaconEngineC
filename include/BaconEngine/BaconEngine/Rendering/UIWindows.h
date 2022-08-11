// Purpose: Every built in UI window.
// Created on: 8/11/2022 @ 1:06 PM

#pragma once

#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Math/Vector.h"
#include "UI.h"

CPP_GUARD_START()
    /**
     * @note
     * Special buttons are buttons that do the main action (i.e. delete a file)
     * Special buttons render towards the right side of the UI. (aka your right)
     * Normal buttons render towards the left side of the UI. (aka your left)
     */
    typedef struct {
        const char* specialButton1;
        const char* specialButton2;
        const char* specialButton3;
        const char* normalButton1;
        const char* normalButton2;
        const char* normalButton3;
    } UIMessageBoxChoices;
    // TODO: Button color.

    typedef enum {
        UI_MESSAGE_BOX_FLAG_NULL,
        UI_MESSAGE_BOX_FLAG_NOT_MOVABLE,
        UI_MESSAGE_BOX_FLAG_NO_TITLE_BAR = (1 << 2),
        UI_MESSAGE_BOX_FLAG_NO_BORDER = (1 << 4),
        UI_MESSAGE_BOX_FLAG_NO_MINIMIZE = (1 << 8)
    } UIMessageBoxFlags;

    void RegisterUIMessageBox(const char* name, const char* description, Vector2I position, UIMessageBoxFlags flags, UIMessageBoxChoices choices);
CPP_GUARD_END()
