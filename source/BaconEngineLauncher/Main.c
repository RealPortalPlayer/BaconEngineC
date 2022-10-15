#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/Launcher.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <SharedEngineCode/OSUser.h>

SEC_CPP_SUPPORT_GUARD_START()
int BE_EntryPoint_StartBaconEngine(int, char**);
const char* BE_EntryPoint_GetClientName(void);

int CallLauncherMain(int argc, char** argv) {
    SEC_ArgumentHandler_Initialize(argc, argv);

    SEC_LOGGER_TRACE("Built on: %s\nBuilt for: %s", __TIMESTAMP__, SEC_OPERATINGSYSTEM_NAME);

    if (SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_VERSION, SEC_BUILTINARGUMENTS_VERSION_SHORT, 0)) {
        SEC_LOGGER_INFO("Engine version: 0.1");
        return 0;
    }

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
SEC_CPP_SUPPORT_GUARD_END()