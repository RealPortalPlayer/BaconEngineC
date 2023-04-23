// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/Launcher.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <SharedEngineCode/OSUser.h>
#include <SharedEngineCode/Internal/PlatformSpecific.h>

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <dlfcn.h>
#elif SEC_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#endif

int main(int argc, char** argv) {
    SEC_ArgumentHandler_Initialize(argc, argv);
    SEC_LOGGER_TRACE("Built on: %s\nBuilt for: %s\n", __TIMESTAMP__, SEC_OPERATINGSYSTEM_NAME);

    if (SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_VERSION, SEC_BUILTINARGUMENTS_VERSION_SHORT, 0)) {
        SEC_LOGGER_INFO("Launcher version: %s\n"
                        "Built with engine version: %s\n", BE_LAUNCHER_VERSION, BE_ENGINE_VERSION);

        void* engineBinary = NULL;

        {
            SEC_ArgumentHandler_ShortResults results;

            if (SEC_ArgumentHandler_GetInfoWithShort(SEC_BUILTINARGUMENTS_ENGINE, SEC_BUILTINARGUMENTS_ENGINE_SHORT, SEC_FALSE, &results) != 0)
                engineBinary = SEC_PLATFORMSPECIFIC_GET_BINARY(*results.value, RTLD_NOW);
            else
                engineBinary = SEC_PLATFORMSPECIFIC_GET_BINARY("./BaconEngine" SEC_PLATFORMSPECIFIC_BINARY_EXTENSION, RTLD_NOW);
        }

        if (engineBinary != NULL) {
            const char* (*getVersion)(void) = (const char* (*)(void)) SEC_PLATFORMSPECIFIC_GET_ADDRESS(engineBinary, "BE_EntryPoint_GetVersion");

            if (getVersion != NULL)
                SEC_Logger_LogImplementation(SEC_FALSE, SEC_LOGGER_LOG_LEVEL_INFO, "Engine version: %s\n", getVersion());

            SEC_PLATFORMSPECIFIC_CLOSE_BINARY(engineBinary);
        }

        void* clientBinary = NULL;
        SEC_ArgumentHandler_ShortResults results;

        if (SEC_ArgumentHandler_GetInfoWithShort(SEC_BUILTINARGUMENTS_CLIENT, SEC_BUILTINARGUMENTS_CLIENT_SHORT, 0, &results) == 0 ||
            SEC_PLATFORMSPECIFIC_CHDIR(*results.value) != 0)
            return 0;

        clientBinary = SEC_PLATFORMSPECIFIC_GET_BINARY("./binary" SEC_PLATFORMSPECIFIC_BINARY_EXTENSION, RTLD_NOW);

        if (clientBinary == NULL)
            return 0;

        const char* (*getVersion)(void) = (const char* (*)(void)) SEC_PLATFORMSPECIFIC_GET_ADDRESS(clientBinary, "I_EntryPoint_GetEngineVersion");

        if (getVersion != NULL)
            SEC_Logger_LogImplementation(SEC_FALSE, SEC_LOGGER_LOG_LEVEL_INFO, "Client was compiled with engine version: %s\n", getVersion());

        SEC_PLATFORMSPECIFIC_CLOSE_BINARY(clientBinary);
        return 0;
    }

    if (SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_HELP, SEC_BUILTINARGUMENTS_HELP_SHORT, 0)) {
        SEC_LOGGER_INFO("Arguments:\n%s\n", SEC_Launcher_GetDefaultHelpList());
        return 0;
    }

    if (SEC_OSUser_IsAdmin())
        SEC_LOGGER_WARN("You're running as root! If a client says you require to be root, then it's probably a virus\n");

    SEC_ArgumentHandler_ShortResults results;

    if (SEC_ArgumentHandler_GetInfoWithShort(SEC_BUILTINARGUMENTS_CLIENT, SEC_BUILTINARGUMENTS_CLIENT_SHORT, 0, &results) == 0) {
        SEC_LOGGER_FATAL("No client specified, please check help for more information\n");
        return 1;
    }

    SEC_Launcher_Configuration configuration = {
        .code = SEC_LAUNCHER_ERROR_CODE_NULL
    };

    SEC_Launcher_CreateConfiguration(&configuration, *results.value);

    if (configuration.code != SEC_LAUNCHER_ERROR_CODE_NULL) {
        switch (configuration.code) {
            case SEC_LAUNCHER_ERROR_CODE_BINARY:
                SEC_LOGGER_FATAL("Failed to load %s binary: %s\n", configuration.unionVariables.errorReason.isEngine ? "engine" : "client", configuration.unionVariables.errorReason.errorMessage);
                return 1;

            case SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL:
                SEC_LOGGER_FATAL("Failed to get engine entry-point: %s\n", configuration.unionVariables.errorReason.errorMessage);
                return 1;

            case SEC_LAUNCHER_ERROR_CODE_CHDIR:
                SEC_LOGGER_FATAL("Failed to set current directory: %s\n", configuration.unionVariables.errorReason.errorMessage);
                return 1;

            default:
                SEC_LOGGER_FATAL("Unknown error: %i\n", configuration.code);
                return 1;
        }
    }

    if (configuration.unionVariables.data.clientName[0] != '\0')
        SEC_LOGGER_INFO("Ready, starting: %s\n", configuration.unionVariables.data.clientName);
    else
        SEC_LOGGER_INFO("Ready, starting");

    SEC_LOGGER_TRACE("Entering engine code\n");

    int returnValue = SEC_LAUNCHER_START_ENGINE(configuration, argc, argv);

    SEC_LOGGER_TRACE("Returned back to launcher\n");
    SEC_LOGGER_TRACE("Freeing binaries\n");
    SEC_PLATFORMSPECIFIC_CLOSE_BINARY(configuration.unionVariables.data.engineBinary);
    SEC_PLATFORMSPECIFIC_CLOSE_BINARY(configuration.unionVariables.data.clientBinary);
    SEC_LOGGER_INFO("Goodbye\n");
    return returnValue;
}
