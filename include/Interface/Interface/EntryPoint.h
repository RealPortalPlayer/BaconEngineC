// Purpose: Main entry-point for the client
// Created on: 4/22/23 @ 2:02 PM

// Copyright (c) 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#ifdef I_INCLUDED_THE_ENTRY_POINT
#   error You cannot include the entry-point more than once
#endif

#define I_INCLUDED_THE_ENTRY_POINT

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/Boolean.h>
#include <BaconAPI/Internal/OperatingSystem.h>
#include <BaconAPI/String.h>

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   define I_BINARYEXPORT __attribute__((visibility("default")))
#elif BA_OPERATINGSYSTEM_WINDOWS
#   define I_BINARYEXPORT __declspec(dllexport)
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
void I_EntryPoint_Initialize(const char* launcherPath, const char* enginePath, const char* clientPath, void* binary, int argc, char** argv);

I_BINARYEXPORT void I_EntryPoint_InitializeWrapper(const char* launcherPath, const char* enginePath, const char* clientPath, void* binary, int argc, char** argv) {
    I_EntryPoint_Initialize(launcherPath, enginePath, clientPath, binary, argc, argv);
}

I_BINARYEXPORT const char* I_EntryPoint_GetEngineVersion(void) {
    return BE_ENGINE_VERSION;
}

I_BINARYEXPORT const char* I_EntryPoint_GetEngineName(void) {
    return BE_ENGINE_NAME;
}

I_BINARYEXPORT int I_EntryPoint_Start(int argc, char** argv);
I_BINARYEXPORT int I_EntryPoint_Shutdown(void);
I_BINARYEXPORT BA_Boolean I_EntryPoint_SupportsServer(void);
I_BINARYEXPORT const char* I_EntryPoint_GetName(void);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
