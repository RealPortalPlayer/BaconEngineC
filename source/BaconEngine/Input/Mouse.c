// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconEngine/Input/Mouse.h"

SEC_CPP_SUPPORT_GUARD_START()
int mouseButtonDown[5] = {0}; // TODO: Replace with renderer specific function

void BE_Mouse_SetButtonDown(BE_Mouse_ButtonType button, int down) {
    if (button >= 5)
        return;

    mouseButtonDown[button] = down;
}

int BE_Mouse_IsButtonDown(BE_Mouse_ButtonType button) {
    return button < 5 ? mouseButtonDown[button] : 0;
}
SEC_CPP_SUPPORT_GUARD_END()
