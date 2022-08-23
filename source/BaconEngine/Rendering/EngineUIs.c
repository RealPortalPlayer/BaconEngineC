#include <SharedEngineCode/Internal/CppHeader.h>
#include <SharedEngineCode/Debugging/Assert.h>

#include "EngineUIs.h"
#include "BaconEngine/Rendering/UI.h"

CPP_GUARD_START()
    void InitializeEngineUIs(void) {
        static int initialized = 0;

        SEC_ASSERT(!initialized, "Engine UIs are already layerInitialized");

        initialized = 1;

        BE_RegisterUIWindow("Console", (BE_Vector2I) {5, 5}, (BE_Vector2U) {300, 350}, BE_UI_WINDOW_FLAG_NO_MAXIMIZE);
        BE_FinishRegisteringUIWindow();
    }
CPP_GUARD_END()
