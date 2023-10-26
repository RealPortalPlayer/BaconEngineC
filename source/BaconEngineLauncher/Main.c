// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/Logger.h>
#include <SharedEngineCode/Launcher.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <BaconAPI/User.h>
#include <BaconAPI/Internal/PlatformSpecific.h>

#if BA_OPERATINGSYSTEM_WINDOWS
#   include <stdio.h>
#   include <Windows.h>
#endif

void I_EntryPoint_InitializeWrapper(const char*, const char*, const char*, void*, int, char**);
const char* I_EntryPoint_GetEngineVersion(void);
int I_EntryPoint_Start(int, char**);
int I_EntryPoint_Shutdown(void);
BA_Boolean I_EntryPoint_SupportsServer(void);
const char* I_EntryPoint_GetName(void);

BA_CPLUSPLUS_SUPPORT_GUARD_START()
int main(int argc, char** argv) {
    BA_ArgumentHandler_Initialize(argc, argv);
    BA_LOGGER_TRACE("Built on: %s\nBuilt for: %s\n", __TIMESTAMP__, BA_OPERATINGSYSTEM_NAME);
    BA_LOGGER_DEBUG("Getting engine\n");
    
    SEC_Launcher_Configuration configuration;
    
    configuration.code = SEC_LAUNCHER_ERROR_CODE_NULL;
    
    SEC_Launcher_SetLauncherPath();
    SEC_Launcher_SetEnginePath();
    SEC_Paths_SetClientPath(SEC_Paths_GetLauncherDirectory());
    SEC_Launcher_InitializeEngine(&configuration);

    if (BA_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_VERSION, SEC_BUILTINARGUMENTS_VERSION_SHORT, 0)) {
        BA_LOGGER_INFO("Based on launcher version: %s\n", BE_LAUNCHER_VERSION);
        
        if (configuration.code == SEC_LAUNCHER_ERROR_CODE_NULL) {
            const char* (*getVersion)(void);

            BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(const char* (*)(void), getVersion, BA_PLATFORMSPECIFIC_GET_ADDRESS(configuration.unionVariables.data.engineBinary, "BE_EntryPoint_GetVersion"));
            
            if (getVersion != NULL)
                BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_INFO, "Engine version: %s\n", getVersion());

            BA_PLATFORMSPECIFIC_CLOSE_BINARY(configuration.unionVariables.data.engineBinary);
        }

        BA_Logger_LogImplementation(BA_BOOLEAN_FALSE, BA_LOGGER_LOG_LEVEL_INFO, "Client was compiled with engine version: %s\n", BE_ENGINE_VERSION);
        return 0;
    }

    if (BA_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_HELP, SEC_BUILTINARGUMENTS_HELP_SHORT, 0)) {
        BA_LOGGER_INFO("Arguments:\n%s\n", SEC_Launcher_GetDefaultHelpList());
        return 0;
    }

    if (BA_User_IsAdministrator())
        BA_LOGGER_WARN("You're running as root! If a client says you require to be root, then it's probably a virus\n");

    if (configuration.code != SEC_LAUNCHER_ERROR_CODE_NULL) {
        switch (configuration.code) {
            case SEC_LAUNCHER_ERROR_CODE_BINARY:
                BA_LOGGER_FATAL("Failed to load engine binary: %s\n", configuration.unionVariables.errorReason.errorMessage);
                return 1;

            case SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL:
                BA_LOGGER_FATAL("Failed to get engine entry-point: %s\n", configuration.unionVariables.errorReason.errorMessage);
                return 1;

            default:
                BA_LOGGER_FATAL("Unknown error: %i\n", configuration.code);
                return 1;
        }
    }

    configuration.unionVariables.data.clientBinary = BA_PLATFORMSPECIFIC_GET_BINARY(NULL, RTLD_NOW);

    {
        const char* (*getName)(void);

        BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(const char* (*)(void), getName, BA_PLATFORMSPECIFIC_GET_ADDRESS(configuration.unionVariables.data.clientBinary, "I_EntryPoint_GetName"));
        
        if (getName != NULL)
            BA_LOGGER_INFO("Ready, starting: %s\n", getName());
        else
            BA_LOGGER_INFO("Ready, starting\n");
    }
    
    BA_LOGGER_TRACE("Entering engine code\n");

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

    BA_LOGGER_TRACE("Returned back to launcher\n");
    BA_LOGGER_TRACE("Freeing engine binary\n");
    BA_PLATFORMSPECIFIC_CLOSE_BINARY(configuration.unionVariables.data.engineBinary);
    BA_LOGGER_INFO("Goodbye\n");
    return returnValue;
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
