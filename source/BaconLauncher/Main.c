#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/Launcher.h>
#include <stddef.h>

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <dlfcn.h>
#   include <unistd.h>
#elif SEC_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#endif

int main(int argc, char** argv) {
    SEC_ArgumentHandler_Initialize(argc, argv);

    SEC_LOGGER_TRACE("Built: %s", __TIMESTAMP__);

    if (SEC_ArgumentHandler_GetIndex("--help") != -1) {
        SEC_LOGGER_INFO("Arguments:\n%s", SEC_Launcher_GetDefaultHelpList());
        return 0;
    }

    // TODO: OS_WINDOWS
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    if (getuid() == 0)
        SEC_LOGGER_WARN("You're running as root! If a client says you require to be root, then it's probably a virus");
#endif

    const char* clientPath = SEC_ArgumentHandler_GetValue("--client");

    if (clientPath == NULL)
        clientPath = SEC_ArgumentHandler_GetValue("-c");

    if (clientPath == NULL) {
        SEC_LOGGER_WARN("You didn't specify what client you wanted to open, defaulting to 'Client'");

        clientPath = "./Client";
    }

    SEC_LOGGER_INFO("Getting configuration information");

    SEC_Launcher_Configuration configuration = {
        .code = SEC_LAUNCHER_ERROR_CODE_NULL
    };

    SEC_Launcher_CreateConfiguration(&configuration, clientPath);

    if (configuration.code != SEC_LAUNCHER_ERROR_CODE_NULL) {
        switch (configuration.code) {
            case SEC_LAUNCHER_ERROR_CODE_BINARY:
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
                SEC_LOGGER_FATAL("Failed to load the binary: %s", configuration.errorMessage);
#elif SEC_OPERATINGSYSTEM_WINDOWS
                SEC_LOG_FATAL("Failed to load the binary: %i", configuration.code);
#endif
                return 1;

            case SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL:
            case SEC_LAUNCHER_ERROR_CODE_NAME_NULL:
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
                SEC_LOGGER_FATAL("Failed to get important methods: %s", configuration.errorMessage);
#elif SEC_OPERATINGSYSTEM_WINDOWS
                SEC_LOG_FATAL("Failed to get important methods: %i", configuration.code);
#endif
                return 1;

            default:
                SEC_LOGGER_FATAL("Unknown error: %i", configuration.code);
                return 1;
        }
    }

    SEC_LOGGER_INFO("Ready, starting '%s'", configuration.clientName);
    SEC_LOGGER_TRACE("Entering engine code");

    int returnValue = configuration.Start(argc, argv);

    SEC_LOGGER_TRACE("Returned back to launcher");
    SEC_LOGGER_TRACE("Freeing binaries");

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    dlclose(configuration.clientBinary);
#elif SEC_OPERATINGSYSTEM_WINDOWS
    FreeLibrary(configuration.clientBinary);
#endif

    SEC_LOGGER_INFO("Goodbye");

    return returnValue;
}