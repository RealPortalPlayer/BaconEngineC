#include <SharedEngineCode/Internal/CppHeader.h>
#include <SharedEngineCode/Debugging/Assert.h>

#include "EngineUIs.h"
#include "BaconEngine/Rendering/UI.h"

SEC_CPP_GUARD_START()
    void InitializeEngineUIs(void) {
        static int initialized = 0;

        SEC_ASSERT(!initialized, "Engine UIs are already initialized");

        initialized = 1;

        BE_UI_RegisterWindow("Console", (BE_Vector_2I) {5, 5}, (BE_Vector_2U) {300, 350}, BE_UI_WINDOW_FLAG_NO_MAXIMIZE);
        BE_UI_FinishRegisteringWindow();
    }
SEC_CPP_GUARD_END()
