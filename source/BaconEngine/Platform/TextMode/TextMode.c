// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "SharedEngineCode/Internal/CppSupport.h"

#include "TextMode.h"
#include "../SpecificPlatformFunctions.h"
#include "BaconEngine/Debugging/Assert.h"
#include "TextModeWindow.h"
#include "BaconEngine/ClientInformation.h"

SEC_CPP_SUPPORT_GUARD_START()
double BE_TextMode_GetTimer(void) {
    return 0;
}

void BE_TextMode_Initialize(void) {
    BE_SpecificPlatformFunctions_InitializeWindow((BE_SpecificPlatformFunctions_Window) {
            &BE_TextModeWindow_NoOperation,
            (void (*)(const char *, BE_Vector_2U, int)) &BE_TextModeWindow_NoOperation,
            (const char *(*)(void)) &BE_TextModeWindow_NoOperation,
            (BE_Vector_2U (*)(void)) &BE_TextModeWindow_NoOperation,
            (BE_Vector_2I (*)(void)) &BE_TextModeWindow_NoOperation,
            (int (*)(void)) &BE_TextModeWindow_NoOperation,
            &BE_ClientInformation_IsRunning,
            (void (*)(const char *)) &BE_TextModeWindow_NoOperation,
            (void (*)(BE_Vector_2U)) &BE_TextModeWindow_NoOperation,
            (void (*)(BE_Vector_2I)) &BE_TextModeWindow_NoOperation,
            (void (*)(int)) &BE_TextModeWindow_NoOperation,
            (void (*)(void)) &BE_TextModeWindow_NoOperation,
            (void (*)(void)) &BE_TextModeWindow_NoOperation,
    });
    BE_SpecificPlatformFunctions_InitializeRenderer((BE_SpecificPlatformFunctions_Renderer) {
            (void (*)(void)) &BE_TextModeWindow_NoOperation,
            (void (*)(BE_Color_3U)) &BE_TextModeWindow_NoOperation,
            (BE_Color_3U (*)(void)) &BE_TextModeWindow_NoOperation,
            (void (*)(void)) &BE_TextModeWindow_NoOperation,
            (void (*)(BE_Vector_2I, BE_Vector_2U, BE_Color_4U)) &BE_TextModeWindow_NoOperation,
    });
    BE_SpecificPlatformFunctions_SetDestroy(&BE_TextMode_Destroy);
    BE_SpecificPlatformFunctions_SetGetTimer(&BE_TextMode_GetTimer);
}

void BE_TextMode_Destroy(void) {
}
SEC_CPP_SUPPORT_GUARD_END()
