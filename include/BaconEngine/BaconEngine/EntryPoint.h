// Purpose: The entry point for the game engine.
// Created on: 3/29/22 @ 12:33 AM

// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/Internal/CppSupport.h>
#include <SharedEngineCode/Internal/OperatingSystem.h>
#include <SharedEngineCode/Internal/Boolean.h>

#ifdef BACON_ENGINE_INCLUDED_ENTRY_POINT
#   error You cannot include the entry point more than once.
#endif

#define BACON_ENGINE_INCLUDED_ENTRY_POINT

#ifndef BACON_ENGINE_LAUNCHER
#   if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#       define BE_ENTRYPOINT_EXPOSE_FUNC __attribute__((visibility("default")))
#   elif SEC_OPERATINGSYSTEM_WINDOWS
#       define BE_ENTRYPOINT_EXPOSE_FUNC __declspec(dllexport)
#   endif
#else
#   define BE_ENTRYPOINT_EXPOSE_FUNC
#endif

SEC_CPP_SUPPORT_GUARD_START()
#ifdef BACON_ENGINE_LAUNCHER
int CallLauncherMain(int argc, char** argv);

#   if SEC_OPERATINGSYSTEM_WINDOWS
#       define WIN32_LEAN_AND_MEAN
#       include <Windows.h> // FIXME: I really don't want to include this in a header (especially a header like this one), but it seems like the only way.
#       include <stdlib.h>
BOOL WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand) {
    return CallLauncherMain(__argc, __argv);
}
#   else
int main(int argc, char** argv) {
    return CallLauncherMain(argc, argv);
}
#   endif
#endif

int BE_EntryPoint_ClientStart(int argc, char** argv);
int BE_EntryPoint_ClientShutdown(void);
BE_ENTRYPOINT_EXPOSE_FUNC SEC_Boolean BE_EntryPoint_ClientSupportsServer(void);
BE_ENTRYPOINT_EXPOSE_FUNC const char* BE_EntryPoint_GetClientName(void);
int BE_EntryPoint_StartBaconEngine(int argc, char** argv);

BE_ENTRYPOINT_EXPOSE_FUNC int BE_EntryPoint_InitializeDynamicLibrary(int argc, char** argv) {
    return BE_EntryPoint_StartBaconEngine(argc, argv);
}
SEC_CPP_SUPPORT_GUARD_END()
