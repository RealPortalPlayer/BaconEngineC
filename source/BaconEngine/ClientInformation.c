#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Internal/CppHeader.h>
#include <SharedEngineCode/Logger.h>

#include "BaconEngine/ClientInformation.h"

SEC_CPP_GUARD_START()
    int running = 1;
    int cheats = 0;

    int BE_ClientInformation_IsRunning(void) {
        return running;
    }

    int BE_ClientInformation_IsCheatsEnabled(void) {
        return cheats;
    }

    int BE_ClientInformation_IsServerModeEnabled(void) {
        static int enabled = -1;

        if (enabled == -1)
            enabled = SEC_ArgumentHandler_GetIndex("--server") != -1 || SEC_ArgumentHandler_GetIndex("-s") != -1;

        return enabled;
    }

    int BE_ClientInformation_IsStrictModeEnabled(void) {
        static int enabled = -1;

        if (enabled == -1)
            enabled = SEC_ArgumentHandler_GetIndex("--no-strict") == -1 && SEC_ArgumentHandler_GetIndex("-ns") == -1;

        return enabled;
    }

    void BE_ClientInformation_StopRunning(void) {
        running = 0;
    }

    void BE_ClientInformation_SetCheats(int enable) {
        cheats = enable;
    }
SEC_CPP_GUARD_END()
