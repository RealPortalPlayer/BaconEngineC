#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/Launcher.h>
#include <SharedEngineCode/BuiltInArguments.h>
#include <SharedEngineCode/OSUser.h>
#include <SharedEngineCode/MessageBox.h>

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <dlfcn.h>
#elif SEC_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#   include <stdlib.h>
#   include <stdio.h>
#   define argc __argc
#   define argv __argv
#endif

#define LOG_FATAL_LOG_MESSAGE_BOX(message) \
do {                                       \
    SEC_LOGGER_FATAL(message "\n");        \
    SEC_MessageBox_Display(message, "Launcher - Fatal Error", SEC_MESSAGEBOX_ICON_ERROR, SEC_MESSAGEBOX_BUTTON_OK); \
} while (0)

#if SEC_OPERATINGSYSTEM_WINDOWS
BOOL WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand) {
#else
int main(int argc, char** argv) {
#endif
    SEC_ArgumentHandler_Initialize(argc, argv);

#if SEC_OPERATINGSYSTEM_WINDOWS
    FILE* consoleHandle = NULL;

    if (SEC_ArgumentHandler_GetIndex(SEC_BUILTINARGUMENTS_SHOW_TERMINAL, 0) != -1 && AllocConsole()) {
        SetConsoleTitle("BaconEngine");
        freopen_s(&consoleHandle, "CONIN$", "r", stdin);
        freopen_s(&consoleHandle, "CONOUT$", "w", stderr);
        freopen_s(&consoleHandle, "CONOUT$", "w", stdout);
    }
#endif

    SEC_LOGGER_TRACE("Built on: %s\nBuilt for: %s\n", __TIMESTAMP__, SEC_OPERATINGSYSTEM_NAME);

    if (SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_VERSION, SEC_BUILTINARGUMENTS_VERSION_SHORT, 0)) {
        SEC_LOGGER_INFO("Launcher version: 0.1\n");
        SEC_MessageBox_Display("Version: 0.1", "Launcher - Info", SEC_MESSAGEBOX_ICON_INFORMATION, SEC_MESSAGEBOX_BUTTON_OK);
        return 0;
    }

    if (SEC_ArgumentHandler_GetIndex(SEC_BUILTINARGUMENTS_HELP, 0) != -1) {
        SEC_LOGGER_INFO("Arguments:\n%s\n", SEC_Launcher_GetDefaultHelpList());
        SEC_MessageBox_Display(SEC_Launcher_GetDefaultHelpList(), "Launcher - Info", SEC_MESSAGEBOX_ICON_INFORMATION, SEC_MESSAGEBOX_BUTTON_OK);
        return 0;
    }

    if (SEC_OSUser_IsAdmin())
        SEC_LOGGER_WARN("You're running as root! If a client says you require to be root, then it's probably a virus\n");

    SEC_LOGGER_INFO("Getting configuration information\n");

    SEC_ArgumentHandler_ShortResults results;

    if (SEC_ArgumentHandler_GetInfoWithShort(SEC_BUILTINARGUMENTS_CLIENT, SEC_BUILTINARGUMENTS_CLIENT_SHORT, 0, &results) == 0) {
        LOG_FATAL_LOG_MESSAGE_BOX("No client specified, please check help for more information");
        return 1;
    }

    SEC_Launcher_Configuration configuration = {
        .code = SEC_LAUNCHER_ERROR_CODE_NULL
    };

    SEC_Launcher_CreateConfiguration(&configuration, *results.value);

    if (configuration.code != SEC_LAUNCHER_ERROR_CODE_NULL) { // TODO: Parse Windows error code into string.
        switch (configuration.code) {
            case SEC_LAUNCHER_ERROR_CODE_BINARY:
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
                SEC_LOGGER_FATAL("Failed to load the binary: %s\n", configuration.errorMessage);
#elif SEC_OPERATINGSYSTEM_WINDOWS
                SEC_LOGGER_FATAL("Failed to load the binary: %i\n", configuration.code);
#endif
                SEC_MessageBox_Display("Failed to load binary", "Launcher - Fatal Error", SEC_MESSAGEBOX_ICON_ERROR, SEC_MESSAGEBOX_BUTTON_OK);
                return 1;

            case SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL:
            case SEC_LAUNCHER_ERROR_CODE_NAME_NULL:
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
                SEC_LOGGER_FATAL("Failed to get important methods: %s\n", configuration.errorMessage);
#elif SEC_OPERATINGSYSTEM_WINDOWS
                SEC_LOGGER_FATAL("Failed to get important methods: %i\n", configuration.code);
#endif
                SEC_MessageBox_Display("Failed to get important methods", "Launcher - Fatal Error", SEC_MESSAGEBOX_ICON_ERROR, SEC_MESSAGEBOX_BUTTON_OK);
                return 1;

            case SEC_LAUNCHER_ERROR_CODE_CHDIR:
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
                SEC_LOGGER_FATAL("Failed to set current directory: %s\n", configuration.errorMessage);
#elif SEC_OPERATINGSYSTEM_WINDOWS
                SEC_LOGGER_FATAL("Failed to set current directory: %i\n", configuration.code);
#endif
                SEC_MessageBox_Display("Failed to access client directory", "Launcher - Fatal Error", SEC_MESSAGEBOX_ICON_ERROR, SEC_MESSAGEBOX_BUTTON_OK);
                return 1;


            default:
                SEC_LOGGER_FATAL("Unknown error: %i\n", configuration.code);
                SEC_MessageBox_Display("An unknown error occurred, perhaps your launcher is too old", "Launcher - Unknown Error", SEC_MESSAGEBOX_ICON_ERROR, SEC_MESSAGEBOX_BUTTON_OK);
                return 1;
        }
    }

    SEC_LOGGER_INFO("Ready, starting '%s'\n", configuration.clientName);
    SEC_LOGGER_TRACE("Entering engine code\n");

    int returnValue = configuration.Start(argc, argv);

    SEC_LOGGER_TRACE("Returned back to launcher\n");
    SEC_LOGGER_TRACE("Freeing binaries\n");

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    dlclose(configuration.clientBinary);
#elif SEC_OPERATINGSYSTEM_WINDOWS
    FreeLibrary(configuration.clientBinary);
#endif

    SEC_LOGGER_INFO("Goodbye\n");

#if SEC_OPERATINGSYSTEM_WINDOWS
    if (consoleHandle != NULL) {
        fclose(consoleHandle);
        FreeConsole();
    }
#endif

    return returnValue;
}