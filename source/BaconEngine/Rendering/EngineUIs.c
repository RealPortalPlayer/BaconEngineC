// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <SharedEngineCode/Debugging/Assert.h>

#include "EngineUIs.h"
#include "BaconEngine/Rendering/UI.h"
#include "BaconEngine/Math/Bitwise.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
void BE_EngineUIs_Initialize(void) {
    static SEC_Boolean initialized = SEC_BOOLEAN_FALSE;

    SEC_ASSERT(!initialized, "Engine UIs are already initialized\n");

    initialized = SEC_BOOLEAN_TRUE;

    BE_UI_WindowFlags flags = BE_UI_WINDOW_FLAG_NO_MAXIMIZE | BE_UI_WINDOW_FLAG_NO_MINIMIZE;

    if (SEC_ArgumentHandler_GetIndex(SEC_BUILTINARGUMENTS_CONSOLE, 0) == -1)
        BE_BITWISE_SET_BIT(flags, BE_UI_WINDOW_FLAG_CLOSED);

    BE_UI_RegisterWindow("Console", flags, SEC_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Integer, 5, 5),
                         SEC_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Unsigned, 300, 350));
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
