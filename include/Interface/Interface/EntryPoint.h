// Purpose: Main entry-point for the client
// Created on: 4/22/2023 @ 2:02 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#ifdef I_INCLUDED_THE_ENTRY_POINT
#   error You cannot include the entry-point more than once
#endif

#define I_INCLUDED_THE_ENTRY_POINT

#include <SharedEngineCode/Internal/CppSupport.h>
#include <SharedEngineCode/Internal/Boolean.h>
#include <SharedEngineCode/Internal/OperatingSystem.h>

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   define I_DLLEXPORT __attribute__((visibility("default")))
#elif SEC_OPERATINGSYSTEM_WINDOWS
#   define I_DLLEXPORT __declspec(dllexport)
#endif

SEC_CPP_SUPPORT_GUARD_START()
void I_EntryPoint_Initialize(void* binary, int argc, char** argv);

I_DLLEXPORT void I_EntryPoint_InitializeWrapper(void* binary, int argc, char** argv) {
    I_EntryPoint_Initialize(binary, argc, argv);
}

I_DLLEXPORT const char* I_EntryPoint_GetEngineVersion(void) {
    return BE_ENGINE_VERSION;
}

I_DLLEXPORT int I_EntryPoint_Start(int argc, char** argv);
I_DLLEXPORT int I_EntryPoint_Shutdown(void);
I_DLLEXPORT SEC_Boolean I_EntryPoint_SupportsServer(void);
I_DLLEXPORT const char* I_EntryPoint_GetName(void);
SEC_CPP_SUPPORT_GUARD_END()
