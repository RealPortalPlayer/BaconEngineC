// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/Debugging/Assert.h>

#include "TextMode.h"
#include "../SpecificPlatformFunctions.h"
#include "BaconEngine/ClientInformation.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
void* BE_TextMode_PtrNoOperation(void) {
    return NULL;
}

BE_Vector2_Unsigned BE_TextMode_Vector2NoOperation(void) {
    return SEC_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Unsigned, 0, 0);
}

int BE_TextMode_IntNoOperation(void) {
    return 0;
}

BE_Color3_Unsigned BE_TextMode_Color3NoOperation(void) {
    return SEC_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Color3_Unsigned, 0, 0, 0);
}

void BE_TextMode_Initialize(void) {
    BE_SpecificPlatformFunctions_Initialize(SEC_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_SpecificPlatformFunctions,
        {
            (void (*)(const char*, BE_Vector2_Unsigned, int)) &BE_TextMode_PtrNoOperation,
            (const char*(*)(void)) &BE_TextMode_PtrNoOperation,
            &BE_TextMode_Vector2NoOperation,
            (BE_Vector2_Integer (*)(void)) &BE_TextMode_Vector2NoOperation,
            &BE_TextMode_IntNoOperation,
            &BE_ClientInformation_IsRunning,
            (void (*)(const char*)) &BE_TextMode_IntNoOperation,
            (void (*)(BE_Vector2_Unsigned)) &BE_TextMode_IntNoOperation,
            (void (*)(BE_Vector2_Integer)) &BE_TextMode_IntNoOperation,
            (void (*)(int)) &BE_TextMode_IntNoOperation,
            (void (*)(void)) &BE_TextMode_IntNoOperation,
            (void (*)(void)) &BE_TextMode_IntNoOperation
        },
        {
            (void (*)(void)) &BE_TextMode_IntNoOperation,
            (void (*)(BE_Color3_Unsigned)) &BE_TextMode_IntNoOperation,
            (BE_Color3_Unsigned (*)(void)) &BE_TextMode_Color3NoOperation,
            (void (*)(void)) &BE_TextMode_IntNoOperation,
            (void (*)(BE_Vector2_Integer, BE_Vector2_Unsigned, BE_Color4_Unsigned)) &BE_TextMode_IntNoOperation,
        },
        {
            (SEC_Boolean (*)(BE_Keyboard_KeyCodes)) &BE_TextMode_IntNoOperation,
            (SEC_Boolean (*)(BE_Keyboard_KeyCodes)) &BE_TextMode_IntNoOperation,
            (SEC_Boolean (*)(BE_Mouse_ButtonType)) &BE_TextMode_IntNoOperation
        },
        (void (*)(void)) &BE_TextMode_IntNoOperation,
        (double (*)(void)) &BE_TextMode_IntNoOperation
    ));
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
