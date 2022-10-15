#include "WindowsRenderer.h"

SEC_CPP_SUPPORT_GUARD_START()
void BE_WindowsRenderer_CompileShaders(void) {

}

void BE_WindowsRenderer_ClearScreen(void) {}

void BE_WindowsRenderer_SetClearColor(BE_Color_3U color) {}

BE_Color_3U BE_WindowsRenderer_GetClearColor(void) {
    return SEC_CPP_SUPPORT_CREATE_STRUCT(BE_Color_3U, 0, 0, 0);
}

void BE_WindowsRenderer_Render(void) {}

void BE_WindowsRenderer_DrawFilledRectangle(BE_Vector_2I position, BE_Vector_2U size, BE_Color_4U color) {}
SEC_CPP_SUPPORT_GUARD_END()
