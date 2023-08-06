// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/Internal/CPlusPlusSupport.h>
#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Paths.h>
#include <SharedEngineCode/Logger.h>

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
static void* iEntryPointEngineBinary;

void* I_Engine_GetBinary(void) {
    return iEntryPointEngineBinary;
}

void I_EntryPoint_Initialize(const char* launcherPath, const char* enginePath, const char* clientPath, void* binary, int argc, char** argv) {
    SEC_ArgumentHandler_Initialize(argc, argv);
    SEC_LOGGER_DEBUG("Hello, from client interface initializer\n");
    SEC_LOGGER_DEBUG("Setting important variables\n");
    SEC_Paths_SetLauncherPath(launcherPath);
    SEC_Paths_SetEnginePath(enginePath);
    SEC_Paths_SetClientPath(clientPath);
    
    iEntryPointEngineBinary = binary;
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
