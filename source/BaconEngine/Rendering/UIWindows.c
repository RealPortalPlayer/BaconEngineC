#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Rendering/UIWindows.h"

CPP_GUARD_START()
    void RegisterUIMessageBox(const char* name, const char* description, Vector2I position, UIMessageBoxFlags flags, UIMessageBoxChoices choices) {
        (void) choices;
        (void) description;
        // TODO: Show Description.
        // TODO: Show Choices.
        // TODO: Calculate Size.

        UIWindowFlags uiFlags = UI_WINDOW_FLAG_NO_MAXIMIZE | UI_WINDOW_FLAG_NOT_RESIZABLE;

        if ((flags & UI_MESSAGE_BOX_FLAG_NOT_MOVABLE) != 0)
            uiFlags |= UI_WINDOW_FLAG_NOT_MOVABLE;

        if ((flags & UI_MESSAGE_BOX_FLAG_NO_TITLE_BAR) != 0)
            uiFlags |= UI_WINDOW_FLAG_NO_TITLE_BAR;

        if ((flags & UI_MESSAGE_BOX_FLAG_NO_BORDER) != 0)
            uiFlags |= UI_WINDOW_FLAG_NO_BORDER;

        if ((flags & UI_MESSAGE_BOX_FLAG_NO_MINIMIZE) != 0)
            uiFlags |= UI_WINDOW_FLAG_NO_MINIMIZE;

        RegisterUIWindow(name, position, (Vector2U) {200, 70}, uiFlags);
    }
CPP_GUARD_END()
