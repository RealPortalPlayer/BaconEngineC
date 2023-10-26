// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/ArgumentHandler.h>
#include <SharedEngineCode/Paths.h>
#include <BaconAPI/Logger.h>

BA_CPLUSPLUS_SUPPORT_GUARD_START()
static void* iEntryPointEngineBinary;

void* I_Engine_GetBinary(void) {
    return iEntryPointEngineBinary;
}

void I_EntryPoint_Initialize(const char* launcherPath, const char* enginePath, const char* clientPath, void* binary, int argc, char** argv) {
    BA_ArgumentHandler_Initialize(argc, argv);
    BA_LOGGER_DEBUG("Hello, from client interface initializer\n");
    BA_LOGGER_DEBUG("Setting important variables\n");
    SEC_Paths_SetLauncherPath(launcherPath);
    SEC_Paths_SetEnginePath(enginePath);
    SEC_Paths_SetClientPath(clientPath);
    
    iEntryPointEngineBinary = binary;
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
