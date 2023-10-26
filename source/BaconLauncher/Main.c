// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
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

    BA_ArgumentHandler_ShortResults results;
    int clientInformationResults = BA_ArgumentHandler_GetInformationWithShort(SEC_BUILTINARGUMENTS_CLIENT, SEC_BUILTINARGUMENTS_CLIENT_SHORT, 0,
                                                                               &results);

    if (BA_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_VERSION, SEC_BUILTINARGUMENTS_VERSION_SHORT, 0)) {
        char* message = BA_String_Copy("Launcher version: %s\n"
                                        "Built with engine version: %s\n");

        BA_String_Format(&message, BE_LAUNCHER_VERSION, BE_ENGINE_VERSION);
        SEC_Launcher_SetLauncherPath();
        SEC_Launcher_SetEnginePath();
        
        if (clientInformationResults != 0)
            SEC_Paths_SetClientPath(*results.value);
        
        SEC_Launcher_InitializeEngine(&configuration);

        if (configuration.code == SEC_LAUNCHER_ERROR_CODE_NULL) {
            typedef const char* (*GetVersion)(void);
            GetVersion getVersion;

            BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(GetVersion, getVersion, BA_PLATFORMSPECIFIC_GET_ADDRESS(configuration.unionVariables.data.engineBinary, "BE_EntryPoint_GetVersion"));
            
            if (getVersion != NULL) {
                BA_String_Append(&message, "Engine version: %s\n");
                BA_String_Format(&message, getVersion());
            }

            BA_PLATFORMSPECIFIC_CLOSE_BINARY(configuration.unionVariables.data.engineBinary);
        }
        
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

        typedef const char* (*GetVersion)(void);
        GetVersion getVersion;

        BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(GetVersion, getVersion, BA_PLATFORMSPECIFIC_GET_ADDRESS(configuration.unionVariables.data.clientBinary, "I_EntryPoint_GetEngineVersion"));
        
        if (getVersion != NULL) {
            BA_String_Append(&message, "Client was compiled with engine version: %s\n");
            BA_String_Format(&message, getVersion());
        }
        
        BL_MAIN_INFO("Version", "%s", message);
        BA_PLATFORMSPECIFIC_CLOSE_BINARY(configuration.unionVariables.data.clientBinary);
        return 0;
    }

    if (BA_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_HELP, SEC_BUILTINARGUMENTS_HELP_SHORT, BA_BOOLEAN_FALSE)) {
        BA_LOGGER_INFO("Arguments:\n%s\n", SEC_Launcher_GetDefaultHelpList());
        return 0;
    }

    if (BA_User_IsAdministrator())
        BA_LOGGER_WARN("You're running as root! If a client says you require to be root, then it's probably a virus\n");

    if (clientInformationResults == 0) {
        BL_MAIN_ERROR("Launcher Error", "No client specified, please check help for more information\n");
        return 1;
    }

    SEC_Launcher_CreateConfiguration(&configuration, *results.value);

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

    if (configuration.unionVariables.data.clientName[0] != '\0')
        BA_LOGGER_INFO("Ready, starting: %s\n", configuration.unionVariables.data.clientName);
    else
        BA_LOGGER_INFO("Ready, starting\n");

    BA_LOGGER_TRACE("Entering engine code\n");

    SEC_Launcher_StartEngineResults engineResults = SEC_Launcher_StartEngine(&configuration);

    if (!engineResults.success) {
        BA_LOGGER_FATAL("Failed to start engine: %s\n", engineResults.unionVariables.errorMessage);
        return 1;
    }

    BA_LOGGER_TRACE("Returned back to launcher\n");
    BA_LOGGER_TRACE("Freeing binaries\n");
    BA_PLATFORMSPECIFIC_CLOSE_BINARY(configuration.unionVariables.data.engineBinary);
    BA_PLATFORMSPECIFIC_CLOSE_BINARY(configuration.unionVariables.data.clientBinary);
    BA_LOGGER_INFO("Goodbye\n");
    return engineResults.unionVariables.returnCode;
}
