#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Internal/CppHeader.h>
#include <SharedEngineCode/Logger.h>

#include "BaconEngine/ClientInformation.h"

CPP_GUARD_START()
    int running = 1;
    int cheats = 0;
    unsigned allocatedEngineMemory = 0;


    unsigned BE_GetAllocatedEngineMemory(void) {
        return allocatedEngineMemory;
    }

    void BE_AddAllocatedEngineMemory(unsigned size) {
        SEC_LOG_TRACE("Allocated %u bytes of memory", size);

        allocatedEngineMemory += size;
    }

    void BE_RemoveAllocatedEngineMemory(unsigned size) {
        if (allocatedEngineMemory == 0 || allocatedEngineMemory < size)
            return;

        SEC_LOG_TRACE("Deallocated %u bytes of memory", size);

        allocatedEngineMemory -= size;
    }

    int BE_IsClientRunning(void) {
        return running;
    }

    int BE_IsClientCheatsEnabled(void) {
        return cheats;
    }

    int BE_IsServerModeEnabled(void) {
        static int enabled = -1;

        if (enabled == -1)
            enabled = SEC_GetArgumentIndex("--server") != -1 || SEC_GetArgumentIndex("-s") != -1;

        return enabled;
    }

    int BE_IsStrictModeEnabled(void) {
        static int enabled = -1;

        if (enabled == -1)
            enabled = SEC_GetArgumentIndex("--no-strict") == -1 && SEC_GetArgumentIndex("-ns") == -1;

        return enabled;
    }

    void BE_StopClientRunning(void) {
        running = 0;
    }

    void BE_SetClientCheats(int enable) {
        cheats = enable;
    }
CPP_GUARD_END()
