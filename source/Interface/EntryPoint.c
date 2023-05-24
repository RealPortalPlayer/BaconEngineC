// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>
#include <SharedEngineCode/ArgumentHandler.h>

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
static void* iEntryPointEngineBinary;

void* I_Engine_GetBinary(void) {
    return iEntryPointEngineBinary;
}

void I_EntryPoint_Initialize(void* binary, int argc, char** argv) {
    SEC_ArgumentHandler_Initialize(argc, argv);

    iEntryPointEngineBinary = binary;
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
