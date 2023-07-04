// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/Launcher.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <SharedEngineCode/User.h>
#include <SharedEngineCode/Internal/PlatformSpecific.h>

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <dlfcn.h>
#elif SEC_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#endif

int main(int argc, char** argv) {
    SEC_ArgumentHandler_Initialize(argc, argv);
    SEC_LOGGER_TRACE("Built on: %s\nBuilt for: %s\n", __TIMESTAMP__, SEC_OPERATINGSYSTEM_NAME);

    SEC_Launcher_Configuration configuration = {
        .code = SEC_LAUNCHER_ERROR_CODE_NULL
    };

    SEC_ArgumentHandler_ShortResults results;
    int clientInformationResults = SEC_ArgumentHandler_GetInformationWithShort(SEC_BUILTINARGUMENTS_CLIENT, SEC_BUILTINARGUMENTS_CLIENT_SHORT, 0,
                                                                               &results);

    if (SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_VERSION, SEC_BUILTINARGUMENTS_VERSION_SHORT, 0)) {
        SEC_LOGGER_INFO("Launcher version: %s\n"
                        "Built with engine version: %s\n", BE_LAUNCHER_VERSION, BE_ENGINE_VERSION);

        SEC_Launcher_SetLauncherPath();
        SEC_Launcher_SetEnginePath();
        
        if (clientInformationResults != 0)
            SEC_Paths_SetClientPath(*results.value);
        
        SEC_Launcher_InitializeEngine(&configuration);

        if (configuration.code == SEC_LAUNCHER_ERROR_CODE_NULL) {
            const char* (*getVersion)(void) = (const char* (*)(void)) SEC_PLATFORMSPECIFIC_GET_ADDRESS(configuration.unionVariables.data.engineBinary, "BE_EntryPoint_GetVersion");

            if (getVersion != NULL)
                SEC_Logger_LogImplementation(SEC_BOOLEAN_FALSE, SEC_LOGGER_LOG_LEVEL_INFO, "Engine version: %s\n", getVersion());

            SEC_PLATFORMSPECIFIC_CLOSE_BINARY(configuration.unionVariables.data.engineBinary);
        }
        
        if (clientInformationResults == 0)
            return 0;
        
        configuration.code = SEC_LAUNCHER_ERROR_CODE_NULL;

        SEC_Launcher_InitializeClient(&configuration);
        
        if (configuration.code != SEC_LAUNCHER_ERROR_CODE_NULL)
            return 0;

        const char* (*getVersion)(void) = (const char* (*)(void)) SEC_PLATFORMSPECIFIC_GET_ADDRESS(configuration.unionVariables.data.clientBinary, "I_EntryPoint_GetEngineVersion");

        if (getVersion != NULL)
            SEC_Logger_LogImplementation(SEC_BOOLEAN_FALSE, SEC_LOGGER_LOG_LEVEL_INFO, "Client was compiled with engine version: %s\n", getVersion());

        SEC_PLATFORMSPECIFIC_CLOSE_BINARY(configuration.unionVariables.data.clientBinary);
        return 0;
    }

    if (SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_HELP, SEC_BUILTINARGUMENTS_HELP_SHORT, 0)) {
        SEC_LOGGER_INFO("Arguments:\n%s\n", SEC_Launcher_GetDefaultHelpList());
        return 0;
    }

    if (SEC_User_IsAdministrator())
        SEC_LOGGER_WARN("You're running as root! If a client says you require to be root, then it's probably a virus\n");

    if (clientInformationResults == 0) {
        SEC_LOGGER_FATAL("No client specified, please check help for more information\n");
        return 1;
    }

    SEC_Launcher_CreateConfiguration(&configuration, *results.value);

    if (configuration.code != SEC_LAUNCHER_ERROR_CODE_NULL) {
        switch (configuration.code) {
            case SEC_LAUNCHER_ERROR_CODE_BINARY:
                SEC_LOGGER_FATAL("Failed to load %s binary: %s\n", configuration.unionVariables.errorReason.isEngine ? "engine" : "client", configuration.unionVariables.errorReason.errorMessage);
                return 1;

            case SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL:
                SEC_LOGGER_FATAL("Failed to get engine entry-point: %s\n", configuration.unionVariables.errorReason.errorMessage);
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
