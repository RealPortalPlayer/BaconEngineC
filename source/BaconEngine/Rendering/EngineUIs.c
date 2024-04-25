// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <BaconAPI/Debugging/Assert.h>
#include <BaconAPI/Math/Bitwise.h>

#include "EngineUIs.h"
#include "BaconEngine/Rendering/UI.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
void BE_EngineUIs_Initialize(void) {
    static BA_Boolean initialized = BA_BOOLEAN_FALSE;

    BA_ASSERT(!initialized, "Engine UIs are already initialized\n");

    initialized = BA_BOOLEAN_TRUE;

    BE_UI_WindowFlags flags = BE_UI_WINDOW_FLAG_NO_MAXIMIZE | BE_UI_WINDOW_FLAG_NO_MINIMIZE;

    if (BA_ArgumentHandler_GetIndex(SEC_BUILTINARGUMENTS_CONSOLE, 0) == -1)
        BA_BITWISE_SET_BIT(flags, BE_UI_WINDOW_FLAG_CLOSED);

    BE_UI_RegisterWindow("Console", flags, BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Integer, 5, 5),
                         BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(BE_Vector2_Unsigned, 300, 350));
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
