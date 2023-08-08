// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/Launcher.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <SharedEngineCode/User.h>
#include <SharedEngineCode/Internal/PlatformSpecific.h>

#if SEC_OPERATINGSYSTEM_WINDOWS
#   include <stdio.h>
#   include <Windows.h>
#endif

void I_EntryPoint_InitializeWrapper(const char*, const char*, const char*, void*, int, char**);
const char* I_EntryPoint_GetEngineVersion(void);
int I_EntryPoint_Start(int, char**);
int I_EntryPoint_Shutdown(void);
SEC_Boolean I_EntryPoint_SupportsServer(void);
const char* I_EntryPoint_GetName(void);

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
int main(int argc, char** argv) {
    SEC_ArgumentHandler_Initialize(argc, argv);
    SEC_LOGGER_TRACE("Built on: %s\nBuilt for: %s\n", __TIMESTAMP__, SEC_OPERATINGSYSTEM_NAME);
    SEC_LOGGER_DEBUG("Getting engine\n");
    
    SEC_Launcher_Configuration configuration;
    
    configuration.code = SEC_LAUNCHER_ERROR_CODE_NULL;
    
    SEC_Launcher_SetLauncherPath();
    SEC_Launcher_SetEnginePath();
    SEC_Paths_SetClientPath(SEC_Paths_GetLauncherDirectory());
    SEC_Launcher_InitializeEngine(&configuration);

    if (SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_VERSION, SEC_BUILTINARGUMENTS_VERSION_SHORT, 0)) {
        SEC_LOGGER_INFO("Based on launcher version: %s\n", BE_LAUNCHER_VERSION);
        
        if (configuration.code == SEC_LAUNCHER_ERROR_CODE_NULL) {
            const char* (*getVersion)(void);

            SEC_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(const char* (*)(void), getVersion, SEC_PLATFORMSPECIFIC_GET_ADDRESS(configuration.unionVariables.data.engineBinary, "BE_EntryPoint_GetVersion"));
            
            if (getVersion != NULL)
                SEC_Logger_LogImplementation(SEC_BOOLEAN_FALSE, SEC_LOGGER_LOG_LEVEL_INFO, "Engine version: %s\n", getVersion());

            SEC_PLATFORMSPECIFIC_CLOSE_BINARY(configuration.unionVariables.data.engineBinary);
        }

        SEC_Logger_LogImplementation(SEC_BOOLEAN_FALSE, SEC_LOGGER_LOG_LEVEL_INFO, "Client was compiled with engine version: %s\n", BE_ENGINE_VERSION);
        return 0;
    }

    if (SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_HELP, SEC_BUILTINARGUMENTS_HELP_SHORT, 0)) {
        SEC_LOGGER_INFO("Arguments:\n%s\n", SEC_Launcher_GetDefaultHelpList());
        return 0;
    }

    if (SEC_User_IsAdministrator())
        SEC_LOGGER_WARN("You're running as root! If a client says you require to be root, then it's probably a virus\n");

    if (configuration.code != SEC_LAUNCHER_ERROR_CODE_NULL) {
        switch (configuration.code) {
            case SEC_LAUNCHER_ERROR_CODE_BINARY:
                SEC_LOGGER_FATAL("Failed to load engine binary: %s\n", configuration.unionVariables.errorReason.errorMessage);
                return 1;

            case SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL:
                SEC_LOGGER_FATAL("Failed to get engine entry-point: %s\n", configuration.unionVariables.errorReason.errorMessage);
                return 1;

            default:
                SEC_LOGGER_FATAL("Unknown error: %i\n", configuration.code);
                return 1;
        }
    }

    configuration.unionVariables.data.clientBinary = SEC_PLATFORMSPECIFIC_GET_BINARY(NULL, RTLD_NOW);

    {
        const char* (*getName)(void);

        SEC_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(const char* (*)(void), getName, SEC_PLATFORMSPECIFIC_GET_ADDRESS(configuration.unionVariables.data.clientBinary, "I_EntryPoint_GetName"));
        
        if (getName != NULL)
            SEC_LOGGER_INFO("Ready, starting: %s\n", getName());
        else
            SEC_LOGGER_INFO("Ready, starting\n");
    }
    
    SEC_LOGGER_TRACE("Entering engine code\n");

    SEC_Launcher_EngineDetails engineDetails = {
        argc,
        argv,
        SEC_Paths_GetLauncherDirectory(),
        SEC_Paths_GetEngineDirectory(),
        SEC_Paths_GetClientDirectory(),
        configuration.unionVariables.data.engineBinary,
        &I_EntryPoint_InitializeWrapper,
        &I_EntryPoint_Start,
        &I_EntryPoint_Shutdown,
        &I_EntryPoint_SupportsServer,
        &I_EntryPoint_GetName,
        &I_EntryPoint_GetEngineVersion
    };
    
    int returnValue = configuration.unionVariables.data.Start(&engineDetails);

    SEC_LOGGER_TRACE("Returned back to launcher\n");
    SEC_LOGGER_TRACE("Freeing engine binary\n");
    SEC_PLATFORMSPECIFIC_CLOSE_BINARY(configuration.unionVariables.data.engineBinary);
    SEC_LOGGER_INFO("Goodbye\n");
    return returnValue;
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
