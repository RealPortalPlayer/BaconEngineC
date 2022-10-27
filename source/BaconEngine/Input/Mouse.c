// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconEngine/Input/Mouse.h"

SEC_CPP_SUPPORT_GUARD_START()
SEC_Boolean beMouseButtonDown[5] = {SEC_FALSE}; // TODO: Replace with renderer specific function

void BE_Mouse_SetButtonDown(BE_Mouse_ButtonType button, SEC_Boolean down) {
    if (button >= 5)
        return;

    beMouseButtonDown[button] = down;
}

SEC_Boolean BE_Mouse_IsButtonDown(BE_Mouse_ButtonType button) {
    return button < 5 ? beMouseButtonDown[button] : SEC_FALSE;
}
SEC_CPP_SUPPORT_GUARD_END()
