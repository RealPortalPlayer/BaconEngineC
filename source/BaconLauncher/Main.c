// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/Logger.h>
#include <SharedEngineCode/Launcher.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <BaconAPI/User.h>
#include <BaconAPI/Internal/PlatformSpecific.h>
#include <SharedEngineCode/MessageBox.h>
#include <BaconAPI/String.h>

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <dlfcn.h>
#elif BA_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#   ifdef BE_WINDOWS_LAUNCHER
#       define argc __argc
#       define argv __argv
#   endif
#endif

#ifdef BE_STANDALONE_CLIENT
void I_EntryPoint_InitializeWrapper(const char*, const char*, const char*, void*, int, char**);
const char* I_EntryPoint_GetEngineVersion(void);
const char* I_EntryPoint_GetEngineName(void);
int I_EntryPoint_Start(int, char**);
int I_EntryPoint_Shutdown(void);
BA_Boolean I_EntryPoint_SupportsServer(void);
const char* I_EntryPoint_GetName(void);
#endif

#ifndef BE_WINDOWS_LAUNCHER
#   define BL_MAIN_INFO(caption, ...) BA_LOGGER_INFO(__VA_ARGS__)
#   define BL_MAIN_ERROR(caption, ...) BA_LOGGER_FATAL(__VA_ARGS__)
int main(int argc, char** argv) {
#else
#   define BL_MAIN_INFO(caption, ...) SEC_MessageBox_Display(SEC_MESSAGEBOX_ICON_INFORMATION, SEC_MESSAGEBOX_BUTTON_OK, caption, __VA_ARGS__)
#   define BL_MAIN_ERROR(caption, ...) SEC_MessageBox_Display(SEC_MESSAGEBOX_ICON_ERROR, SEC_MESSAGEBOX_BUTTON_OK, caption, __VA_ARGS__)
int APIENTRY WinMain(HINSTANCE instanceHandle, HINSTANCE previousInstanceHandle, PSTR commandLine, int commandShow) {
    (void) instanceHandle;
    (void) previousInstanceHandle;
    (void) commandLine;
    (void) commandShow;
#endif
    BA_ArgumentHandler_Initialize(argc, argv);
    BA_LOGGER_TRACE("Built on: %s\nBuilt for: %s\n", __TIMESTAMP__, BA_OPERATINGSYSTEM_NAME);
    
    SEC_Launcher_Configuration configuration = {
        .code = SEC_LAUNCHER_ERROR_CODE_NULL
    };

#ifndef BE_STANDALONE_CLIENT
    BA_ArgumentHandler_ShortResults results;
    int clientInformationResults = BA_ArgumentHandler_GetInformationWithShort(SEC_BUILTINARGUMENTS_CLIENT, SEC_BUILTINARGUMENTS_CLIENT_SHORT, 0,
                                                                               &results);
#else
    SEC_Launcher_SetLauncherPath();
    SEC_Launcher_SetEnginePath();
    SEC_Paths_SetClientPath(SEC_Paths_GetLauncherDirectory());
    SEC_Launcher_InitializeEngine(&configuration);

    configuration.unionVariables.data.clientBinary = BA_PLATFORMSPECIFIC_GET_BINARY(NULL, RTLD_NOW);
#endif
    
    if (BA_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_VERSION, SEC_BUILTINARGUMENTS_VERSION_SHORT, 0)) {
        char* message = BA_String_Copy("Launcher version: %s\n"
                                        "Built with engine version: %s %s\n");

        BA_String_Format(&message, BE_LAUNCHER_VERSION, BE_ENGINE_NAME, BE_ENGINE_VERSION);

#ifndef BE_STANDALONE_CLIENT
        SEC_Launcher_SetLauncherPath();
        SEC_Launcher_SetEnginePath();
        
        if (clientInformationResults != 0)
            SEC_Paths_SetClientPath(*results.value);

        SEC_Launcher_InitializeEngine(&configuration);
#endif
        
        if (configuration.code == SEC_LAUNCHER_ERROR_CODE_NULL) {
            const char* (*getName)(void);
            const char* (*getVersion)(void);

            BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(const char* (*)(void), getName, BA_PLATFORMSPECIFIC_GET_ADDRESS(configuration.unionVariables.data.engineBinary, "BE_EntryPoint_GetName"));
            BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(const char* (*)(void), getVersion, BA_PLATFORMSPECIFIC_GET_ADDRESS(configuration.unionVariables.data.engineBinary, "BE_EntryPoint_GetVersion"));
            
            if (getVersion != NULL) {
                BA_String_Append(&message, "Engine version: %s %s\n");
                BA_String_Format(&message, getName != NULL ? getName() : BE_ENGINE_NAME, getVersion());
            }

            BA_PLATFORMSPECIFIC_CLOSE_BINARY(configuration.unionVariables.data.engineBinary);
        }
        
#ifndef BE_STANDALONE_CLIENT
        if (clientInformationResults == 0) {
            BL_MAIN_INFO("Version", "%s", message);
            return 0;
        }
        
        configuration.code = SEC_LAUNCHER_ERROR_CODE_NULL;
        
        SEC_Launcher_InitializeClient(&configuration);
        
        if (configuration.code != SEC_LAUNCHER_ERROR_CODE_NULL) {
            BL_MAIN_INFO("Version", "%s", message);
            return 0;
        }
#endif
        
        const char* (*getName)(void);
        const char* (*getVersion)(void);

#ifndef BE_STANDALONE_CLIENT
        BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(const char* (*)(void), getName, BA_PLATFORMSPECIFIC_GET_ADDRESS(configuration.unionVariables.data.clientBinary, "I_EntryPoint_GetEngineName"));
        BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(const char* (*)(void), getVersion, BA_PLATFORMSPECIFIC_GET_ADDRESS(configuration.unionVariables.data.clientBinary, "I_EntryPoint_GetEngineVersion"));
#else
        getName = &I_EntryPoint_GetEngineName;
        getVersion = &I_EntryPoint_GetEngineVersion;
#endif
        
        if (getVersion != NULL) {
            BA_String_Append(&message, "Client was compiled with engine version: %s %s\n");
            BA_String_Format(&message, getName != NULL ? getName() : BE_ENGINE_NAME, getVersion());
        }
        
        BL_MAIN_INFO("Version", "%s", message);

#ifndef BE_STANDALONE_CLIENT
        BA_PLATFORMSPECIFIC_CLOSE_BINARY(configuration.unionVariables.data.clientBinary);
#endif
        return 0;
    }

    if (BA_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_HELP, SEC_BUILTINARGUMENTS_HELP_SHORT, BA_BOOLEAN_FALSE)) {
        BA_LOGGER_INFO("Arguments:\n%s\n%s\n", BA_ArgumentHandler_GetHelpMessage(), SEC_Launcher_GetDefaultHelpList());
        return 0;
    }

    if (BA_User_IsAdministrator())
        BA_LOGGER_WARN("You're running as root! If a client says you require to be root, then it's probably a virus\n");

#ifndef BE_STANDALONE_CLIENT
    if (clientInformationResults == 0) {
        BL_MAIN_ERROR("Launcher Error", "No client specified, please check help for more information\n");
        return 1;
    }

    SEC_Launcher_CreateConfiguration(&configuration, *results.value);
#endif

    if (configuration.code != SEC_LAUNCHER_ERROR_CODE_NULL) {
        switch (configuration.code) {
            case SEC_LAUNCHER_ERROR_CODE_BINARY:
                BL_MAIN_ERROR("Launcher Error", "Failed to load %s binary: %s\n", configuration.unionVariables.errorReason.isEngine ? "engine" : "client", configuration.unionVariables.errorReason.errorMessage);
                return 1;

            case SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL:
                BL_MAIN_ERROR("Launcher Error", "Failed to get engine entry-point: %s\n", configuration.unionVariables.errorReason.errorMessage);
                return 1;

            default:
                BL_MAIN_ERROR("Launcher Error", "Unknown error: %i\n", configuration.code);
                return 1;
        }
    }

#ifdef BE_STANDALONE_CLIENT
    configuration.unionVariables.data.clientName = I_EntryPoint_GetName();
#endif

    if (configuration.unionVariables.data.clientName[0] != '\0')
        BA_LOGGER_INFO("Ready, starting: %s\n", configuration.unionVariables.data.clientName);
    else
        BA_LOGGER_INFO("Ready, starting unnamed client\n");

    BA_LOGGER_TRACE("Entering engine code\n");

#ifndef BE_STANDALONE_CLIENT
    SEC_Launcher_StartEngineResults engineResults = SEC_Launcher_StartEngine(&configuration);

    if (!engineResults.success) {
        BA_LOGGER_FATAL("Failed to start engine: %s\n", engineResults.unionVariables.errorMessage);
        return 1;
    }
    
    int returnCode = engineResults.unionVariables.returnCode;
#else
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
        &I_EntryPoint_GetEngineVersion,
        &I_EntryPoint_GetEngineName
    };
    
    int returnCode = configuration.unionVariables.data.Start(&engineDetails);
#endif

    BA_LOGGER_TRACE("Returned back to launcher\n");
    BA_LOGGER_TRACE("Freeing binaries\n");
    BA_PLATFORMSPECIFIC_CLOSE_BINARY(configuration.unionVariables.data.engineBinary);
    BA_PLATFORMSPECIFIC_CLOSE_BINARY(configuration.unionVariables.data.clientBinary);
    BA_LOGGER_INFO("Goodbye\n");
    return returnCode;
}
