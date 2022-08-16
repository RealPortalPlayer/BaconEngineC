#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Rendering/UIWindows.h"
#include "BaconEngine/Rendering/Renderer.h"

CPP_GUARD_START()
    void BE_RegisterUIMessageBox(const char* name, const char* description, BE_Vector2I position, BE_UIMessageBoxFlags flags, BE_UIMessageBoxChoices choices) {
        (void) choices;
        (void) description;
        // TODO: Show Description.
        // TODO: Show Choices.

        BE_UIWindowFlags uiFlags = BE_UI_WINDOW_FLAG_NO_MAXIMIZE | BE_UI_WINDOW_FLAG_NOT_RESIZABLE;

        if ((flags & BE_UI_MESSAGE_BOX_FLAG_NOT_MOVABLE) != 0)
            uiFlags |= BE_UI_WINDOW_FLAG_NOT_MOVABLE;

        if ((flags & BE_UI_MESSAGE_BOX_FLAG_NO_TITLE_BAR) != 0)
            uiFlags |= BE_UI_WINDOW_FLAG_NO_TITLE_BAR;

        if ((flags & BE_UI_MESSAGE_BOX_FLAG_NO_BORDER) != 0)
            uiFlags |= BE_UI_WINDOW_FLAG_NO_BORDER;

        if ((flags & BE_UI_MESSAGE_BOX_FLAG_NO_MINIMIZE) != 0)
            uiFlags |= BE_UI_WINDOW_FLAG_NO_MINIMIZE;

        BE_Vector2U size = {200, 70};
        {
            BE_Vector2U textSize = BE_GetRendererFontSize(BE_GetUIWindowFont(), description);

            if (textSize.x > size.x)
                size.x = textSize.x + 34;
        }

        BE_RegisterUIWindow(name, position, size, uiFlags);
        {
            BE_RegisterUIElement(BE_CreateUILabel(description, (BE_Vector2I) {0, 0}));
        }
        BE_FinishRegisteringUIWindow();
    }
CPP_GUARD_END()
