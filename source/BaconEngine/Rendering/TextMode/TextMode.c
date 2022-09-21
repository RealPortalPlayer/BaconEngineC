#include <SharedEngineCode/Internal/CppHeader.h>

#include "TextMode.h"
#include "../SpecificRendererFunctions.h"
#include "BaconEngine/Debugging/Assert.h"
#include "TextModeWindow.h"
#include "BaconEngine/ClientInformation.h"

SEC_CPP_GUARD_START()
    double BE_TextMode_GetTimer(void) {
        return 0;
    }

    void BE_TextMode_Initialize(void) {
        BE_SpecificRendererFunctions_InitializeWindow((BE_SpecificRendererFunctions_Window) {
            &BE_TextModeWindow_NoOperation,
            (void (*)(const char*, BE_Vector_2U, int)) &BE_TextModeWindow_NoOperation,
            (const char* (*)(void)) &BE_TextModeWindow_NoOperation,
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
        BE_SpecificRendererFunctions_InitializeRenderer((BE_SpecificRendererFunctions_Renderer) {
            (void (*)(void)) &BE_TextModeWindow_NoOperation
        });
        BE_SpecificRendererFunctions_SetDestroy(&BE_TextMode_Destroy);
        BE_SpecificRendererFunctions_SetGetTimer(&BE_TextMode_GetTimer);
    }

    void BE_TextMode_Destroy(void) {
    }
SEC_CPP_GUARD_END()
