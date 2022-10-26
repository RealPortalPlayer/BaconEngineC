// Copyright (c) 2022, PortalPlayer
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/Launcher.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <SharedEngineCode/OSUser.h>

#if SEC_OPERATINGSYSTEM_WINDOWS
#   include <stdio.h>
#   include <Windows.h>
#endif

SEC_CPP_SUPPORT_GUARD_START()
int BE_EntryPoint_StartBaconEngine(int, char**);
const char* BE_EntryPoint_GetClientName(void);

int CallLauncherMain(int argc, char** argv) {
    SEC_ArgumentHandler_Initialize(argc, argv);

#if SEC_OPERATINGSYSTEM_WINDOWS
    FILE* consoleHandle;

    if (SEC_ArgumentHandler_GetIndex(SEC_BUILTINARGUMENTS_SHOW_TERMINAL, 0) != -1 && AllocConsole()) {
        SetConsoleTitle("BaconEngine");
        freopen_s(&consoleHandle, "CONIN$", "r", stdin);
        freopen_s(&consoleHandle, "CONOUT$", "w", stderr);
        freopen_s(&consoleHandle, "CONOUT$", "w", stdout);
    }
#endif

    SEC_LOGGER_TRACE("Built on: %s\nBuilt for: %s\n", __TIMESTAMP__, SEC_OPERATINGSYSTEM_NAME);

    if (SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_VERSION, SEC_BUILTINARGUMENTS_VERSION_SHORT, 0)) {
        SEC_LOGGER_INFO("Engine version: 0.1\n");
        return 0;
    }

    if (SEC_ArgumentHandler_GetIndex(SEC_BUILTINARGUMENTS_HELP, 0) != -1) {
        SEC_LOGGER_INFO("Arguments:\n%s\n", SEC_Launcher_GetDefaultHelpList());
        return 0;
    }

    if (SEC_OSUser_IsAdmin())
        SEC_LOGGER_WARN("You're running as root! If a client says you require to be root, then it's probably a virus\n");

    SEC_LOGGER_INFO("Ready, starting '%s'\n", BE_EntryPoint_GetClientName());
    SEC_LOGGER_TRACE("Entering engine code\n");

    int returnValue = BE_EntryPoint_StartBaconEngine(argc, argv);

    SEC_LOGGER_INFO("Goodbye\n");

#if SEC_OPERATINGSYSTEM_WINDOWS
    if (consoleHandle != NULL) {
        fclose(consoleHandle);
        FreeConsole();
    }
#endif

    return returnValue;
}
SEC_CPP_SUPPORT_GUARD_END()