// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "TextMode.h"
#include "../SpecificPlatformFunctions.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/ClientInformation.h"

SEC_CPP_SUPPORT_GUARD_START()
void* BE_TextMode_PtrNoOperation(void) {
    return NULL;
}

BE_Vector_2U BE_TextMode_Vector2NoOperation(void) {
    return SEC_CPP_SUPPORT_CREATE_STRUCT(BE_Vector_2U, 0, 0);
}

int BE_TextMode_IntNoOperation(void) {
    return 0;
}

BE_Color_3U BE_TextMode_Color3NoOperation(void) {
    return SEC_CPP_SUPPORT_CREATE_STRUCT(BE_Color_3U, 0, 0, 0);
}

void BE_TextMode_Initialize(void) {
    BE_SpecificPlatformFunctions_Initialize(SEC_CPP_SUPPORT_CREATE_STRUCT(BE_SpecificPlatformFunctions,
        {
            (void (*)(const char *, BE_Vector_2U, int)) &BE_TextMode_PtrNoOperation,
            (const char *(*)(void)) &BE_TextMode_PtrNoOperation,
            &BE_TextMode_Vector2NoOperation,
            (BE_Vector_2I (*)(void)) &BE_TextMode_Vector2NoOperation,
            &BE_TextMode_IntNoOperation,
            &BE_ClientInformation_IsRunning,
            (void (*)(const char *)) &BE_TextMode_IntNoOperation,
            (void (*)(BE_Vector_2U)) &BE_TextMode_IntNoOperation,
            (void (*)(BE_Vector_2I)) &BE_TextMode_IntNoOperation,
            (void (*)(int)) &BE_TextMode_IntNoOperation,
            (void (*)(void)) &BE_TextMode_IntNoOperation,
            (void (*)(void)) &BE_TextMode_IntNoOperation
        },
        {
            (void (*)(void)) &BE_TextMode_IntNoOperation,
            (void (*)(BE_Color_3U)) &BE_TextMode_IntNoOperation,
            (BE_Color_3U (*)(void)) &BE_TextMode_Color3NoOperation,
            (void (*)(void)) &BE_TextMode_IntNoOperation,
            (void (*)(BE_Vector_2I, BE_Vector_2U, BE_Color_4U)) &BE_TextMode_IntNoOperation,
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
SEC_CPP_SUPPORT_GUARD_END()
