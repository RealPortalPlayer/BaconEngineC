#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Internal/CppHeader.h>
#include "BaconEngine/ClientInformation.h"

CPP_GUARD_START()
    int running = 1;
    int cheats = 0;

    int IsClientRunning(void) {
        return running;
    }

    int IsClientCheatsEnabled(void) {
        return cheats;
    }

    int IsServerModeEnabled(void) {
        static int enabled = -1;

        if (enabled == -1)
            enabled = GetArgumentIndex("--server") != -1 || GetArgumentIndex("-s") != -1;

        return enabled;
    }

    int IsStrictModeEnabled(void) {
        static int enabled = -1;

        if (enabled == -1)
            enabled = GetArgumentIndex("--no-strict") == -1 && GetArgumentIndex("-ns") == -1;

        return enabled;
    }

    void StopClientRunning(void) {
        running = 0;
    }

    void SetClientCheats(int enable) {
        cheats = enable;
    }
CPP_GUARD_END()
