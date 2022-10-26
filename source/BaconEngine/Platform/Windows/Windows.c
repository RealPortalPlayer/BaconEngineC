// Copyright (c) 2022, PortalPlayer
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <Windows.h>

#include "Windows.h"
#include "BaconEngine/Debugging/Assert.h"
#include "../SpecificPlatformFunctions.h"
#include "WindowsWindow.h"
#include "WindowsRenderer.h"

SEC_CPP_SUPPORT_GUARD_START()
static int beWindowsInitialized = 0;

double BE_Windows_GetTimer(void) {
    return GetTickCount();
}

void BE_Windows_Initialize(void) {
    BE_ASSERT(!beWindowsInitialized, "Already initialized Windows\n");
    BE_SpecificPlatformFunctions_InitializeWindow(SEC_CPP_SUPPORT_CREATE_STRUCT(BE_SpecificPlatformFunctions_Window,
        &BE_WindowsWindow_GetInternal,
        &BE_WindowsWindow_Create,
        &BE_WindowsWindow_GetTitle,
        &BE_WindowsWindow_GetSize,
        &BE_WindowsWindow_GetPosition,
        &BE_WindowsWindow_IsVisible,
        &BE_WindowsWindow_IsStillOpened,
        &BE_WindowsWindow_SetTitle,
        &BE_WindowsWindow_SetSize,
        &BE_WindowsWindow_SetPosition,
        &BE_WindowsWindow_SetVisibility,
        &BE_WindowsWindow_Close,
        &BE_WindowsWindow_UpdateEvents
    ));
    BE_SpecificPlatformFunctions_InitializeRenderer(SEC_CPP_SUPPORT_CREATE_STRUCT(BE_SpecificPlatformFunctions_Renderer,
        &BE_WindowsRenderer_ClearScreen,
        &BE_WindowsRenderer_SetClearColor,
        &BE_WindowsRenderer_GetClearColor,
        &BE_WindowsRenderer_Render,
        &BE_WindowsRenderer_DrawFilledRectangle
    ));
    BE_SpecificPlatformFunctions_SetDestroy(&BE_Windows_Destroy);
    BE_SpecificPlatformFunctions_SetGetTimer(&BE_Windows_GetTimer);
}

void BE_Windows_Destroy(void) {}
SEC_CPP_SUPPORT_GUARD_END()
