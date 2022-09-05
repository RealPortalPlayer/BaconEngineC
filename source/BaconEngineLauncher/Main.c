#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/Launcher.h>

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <unistd.h>
#elif SEC_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#endif

int BE_EntryPoint_StartBaconEngine(int, char**);
const char* BE_EntryPoint_GetClientName(void);

int CallLauncherMain(int argc, char** argv) {
    SEC_ArgumentHandler_Initialize(argc, argv);

    if (SEC_ArgumentHandler_GetIndex("--help") != -1) {
        SEC_LOGGER_INFO("Arguments:\n%s", SEC_Launcher_GetDefaultHelpList());
        return 0;
    }

    // TODO: OS_WINDOWS
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    if (getuid() == 0)
        SEC_LOGGER_WARN("You're running as root! If a client says you require to be root, then it's probably a virus");
#endif

    SEC_LOGGER_INFO("Ready, starting '%s'", BE_EntryPoint_GetClientName());
    SEC_LOGGER_TRACE("Entering engine code");

    int returnValue = BE_EntryPoint_StartBaconEngine(argc, argv);

    SEC_LOGGER_INFO("Goodbye");

    return returnValue;
}