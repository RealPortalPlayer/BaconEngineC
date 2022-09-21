#include <SharedEngineCode/Internal/CppHeader.h>
#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/BuiltInArguments.h>

#include "BaconEngine/Debugging/Assert.h"
#include "EngineUIs.h"
#include "BaconEngine/Rendering/UI.h"
#include "BaconEngine/Math/Bitwise.h"

SEC_CPP_GUARD_START()
    void BE_EngineUIs_Initialize(void) {
        static int initialized = 0;

        BE_ASSERT(!initialized, "Engine UIs are already initialized");

        initialized = 1;

        BE_UI_WindowFlags flags = BE_UI_WINDOW_FLAG_NO_MAXIMIZE | BE_UI_WINDOW_FLAG_NO_MINIMIZE;

        if (SEC_ArgumentHandler_GetIndex(SEC_BUILTINARGUMENTS_CONSOLE, 0) == -1)
            BE_BITWISE_SET_BIT(flags, BE_UI_WINDOW_FLAG_CLOSED);

        BE_UI_RegisterWindow("Console", flags, (BE_Vector_2I) {5, 5}, (BE_Vector_2U) {300, 350});
    }
SEC_CPP_GUARD_END()
