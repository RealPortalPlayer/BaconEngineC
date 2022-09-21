#include <SharedEngineCode/Internal/CppHeader.h>

#include "BaconEngine/Input/Mouse.h"

SEC_CPP_GUARD_START()
    int mouseButtonDown[5] = {0};

    void BE_Mouse_SetButtonDown(BE_Mouse_ButtonType button, int down) {
        if (button >= 5)
            return;

        mouseButtonDown[button] = down;
    }

    int BE_Mouse_IsButtonDown(BE_Mouse_ButtonType button) {
        return button < 5 ? mouseButtonDown[button] : 0;
    }
SEC_CPP_GUARD_END()
