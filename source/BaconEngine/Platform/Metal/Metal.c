// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "Metal.h"
#include "../SpecificPlatformFunctions.h"
#include "MetalWindow.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
void BE_Metal_Initialize(void) {
    BE_SpecificPlatformFunctions_Initialize(SEC_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_SpecificPlatformFunctions,
        {
            &BE_MetalWindow_Create,
            &BE_MetalWindow_GetTitle,
            &BE_MetalWindow_GetSize,
            &BE_MetalWindow_GetPosition,
            &BE_MetalWindow_IsVisible,
            &BE_MetalWindow_IsStillOpened,
            &BE_MetalWindow_SetTitle,
            &BE_MetalWindow_SetSize,
            &BE_MetalWindow_SetPosition,
            &BE_MetalWindow_SetVisibility,
            &BE_MetalWindow_Close,
            &BE_MetalWindow_UpdateEvents
        },
        {
            &BE_Metal_Destroy
//            &BE_MetalRenderer_ClearScreen,
//            &BE_MetalRenderer_SetClearColor,
//            &BE_MetalRenderer_GetClearColor,
//            &BE_MetalRenderer_Render,
//            &BE_MetalRenderer_DrawFilledRectangle
        },
        {
            (SEC_Boolean (*)(BE_Keyboard_KeyCodes)) &BE_Metal_Destroy
//            &BE_MetalInput_IsKeyDown,
//            &BE_MetalInput_IsKeyPressed,
//            &BE_MetalInput_IsMouseDown
        },
        &BE_Metal_Destroy,
//        &BE_Metal_GetTimer
    ));
}

void BE_Metal_Destroy(void) {

}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
