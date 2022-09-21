#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/Launcher.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <SharedEngineCode/OSUser.h>

int BE_EntryPoint_StartBaconEngine(int, char**);
const char* BE_EntryPoint_GetClientName(void);

int CallLauncherMain(int argc, char** argv) {
    SEC_ArgumentHandler_Initialize(argc, argv);

    if (SEC_ArgumentHandler_GetIndex(SEC_BUILTINARGUMENTS_HELP, 0) != -1) {
        SEC_LOGGER_INFO("Arguments:\n%s", SEC_Launcher_GetDefaultHelpList());
        return 0;
    }

    if (SEC_OSUser_IsAdmin())
        SEC_LOGGER_WARN("You're running as root! If a client says you require to be root, then it's probably a virus");

    SEC_LOGGER_INFO("Ready, starting '%s'", BE_EntryPoint_GetClientName());
    SEC_LOGGER_TRACE("Entering engine code");

    int returnValue = BE_EntryPoint_StartBaconEngine(argc, argv);

    SEC_LOGGER_INFO("Goodbye");

    return returnValue;
}