#include <SharedEngineCode/Internal/CppHeader.h>

#include "EngineUIs.h"
#include "BaconEngine/Debugging/Assert.h"
#include "BaconEngine/Rendering/UI.h"

CPP_GUARD_START()
    void InitializeEngineUIs(void) {
        static int initialized = 0;

        ASSERT(!initialized, "Engine UIs are already initialized");

        initialized = 1;

        RegisterUIWindow("Console", (Vector2I) {5, 5}, (Vector2U) {300, 350}, UI_WINDOW_FLAG_NO_MAXIMIZE);
    }
CPP_GUARD_END()
