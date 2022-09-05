#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Rendering/UIWindow.h"
#include "BaconEngine/Rendering/Renderer.h"

SEC_CPP_GUARD_START()
    void BE_UIWindow_RegisterMessageBox(const char* name, const char* description, BE_Vector_2I position, BE_UIWindow_MessageBoxFlags flags, BE_UIWindow_MessageBoxChoices choices) {
        (void) choices;
        (void) description;
        // TODO: Show Description.
        // TODO: Show Choices.

        BE_UI_WindowFlags uiFlags = BE_UI_WINDOW_FLAG_NO_MAXIMIZE | BE_UI_WINDOW_FLAG_NOT_RESIZABLE;

        if ((flags & BE_UIWINDOW_MESSAGE_BOX_FLAG_NOT_MOVABLE) != 0)
            uiFlags |= BE_UI_WINDOW_FLAG_NOT_MOVABLE;

        if ((flags & BE_UIWINDOW_MESSAGE_BOX_FLAG_NO_TITLE_BAR) != 0)
            uiFlags |= BE_UI_WINDOW_FLAG_NO_TITLE_BAR;

        if ((flags & BE_UIWINDOW_MESSAGE_BOX_FLAG_NO_BORDER) != 0)
            uiFlags |= BE_UI_WINDOW_FLAG_NO_BORDER;

        if ((flags & BE_UIWINDOW_MESSAGE_BOX_FLAG_NO_MINIMIZE) != 0)
            uiFlags |= BE_UI_WINDOW_FLAG_NO_MINIMIZE;

        BE_Vector_2U size = {200, 70};
        {
            BE_Vector_2U textSize = BE_Renderer_GetFontSize(BE_UI_GetWindowFont(), description);

            if (textSize.x > size.x)
                size.x = textSize.x + 34;
        }

        BE_UI_RegisterWindow(name, position, size, uiFlags);
        {
            BE_UI_RegisterElement(BE_UIElement_CreateLabel(description, (BE_Vector_2I) {0, 0}));
        }
        BE_UI_FinishRegisteringWindow();
    }
SEC_CPP_GUARD_END()
