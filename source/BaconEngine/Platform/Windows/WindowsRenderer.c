// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "WindowsRenderer.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
void BE_WindowsRenderer_CompileShaders(void) {

}

void BE_WindowsRenderer_ClearScreen(void) {}

void BE_WindowsRenderer_SetClearColor(BE_Color3_Unsigned color) {
    (void) color;
}

BE_Color3_Unsigned BE_WindowsRenderer_GetClearColor(void) {
    return SEC_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Color3_Unsigned, 0, 0, 0);
}

void BE_WindowsRenderer_Render(void) {}

void BE_WindowsRenderer_DrawFilledRectangle(BE_Vector2_Integer position, BE_Vector2_Unsigned size, BE_Color4_Unsigned color) {
    (void) position;
    (void) size;
    (void) color;
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
