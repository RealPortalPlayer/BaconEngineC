#include "SharedEngineCode/Internal/CppHeader.h"
#include "SharedEngineCode/Logger.h"

SEC_CPP_GUARD_START()
    unsigned engineMemory;

    unsigned SEC_EngineMemory_GetAllocated(void) {
        return engineMemory;
    }

    void SEC_EngineMemory_AddAllocated(unsigned size) {
        SEC_LOGGER_TRACE("Allocated %u bytes of memory", size);

        engineMemory += size;
    }

    void SEC_EngineMemory_RemoveAllocated(unsigned size) {
        if (engineMemory == 0 || engineMemory < size)
            return;

        SEC_LOGGER_TRACE("Deallocated %u bytes of memory", size);

        engineMemory -= size;
    }
SEC_CPP_GUARD_END()
