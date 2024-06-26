// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/Internal/CPlusPlusSupport.h>
#include <BaconAPI/Internal/PlatformSpecific.h>
#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/BuiltInArguments.h>

#include "SharedEngineCode/Launcher.h"
#include "SharedEngineCode/BuiltInArguments.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
void SEC_Launcher_CreateConfiguration(SEC_Launcher_Configuration* configuration, const char* path) {
    SEC_Launcher_SetLauncherPath();
    SEC_Launcher_SetEnginePath();
    SEC_Paths_SetClientPath(path);
    
    configuration->code = SEC_LAUNCHER_ERROR_CODE_NULL;

    SEC_Launcher_InitializeEngine(configuration);
    
    if (configuration->code != SEC_LAUNCHER_ERROR_CODE_NULL)
        return;

    SEC_Launcher_InitializeClient(configuration);
}

const char* SEC_Launcher_GetDefaultHelpList(void) {
    return BA_ARGUMENTHANDLER_HELP_MESSAGE(SEC_BUILTINARGUMENTS_HELP, SEC_BUILTINARGUMENTS_HELP_SHORT, "Shows information about each argument\n")
           BA_ARGUMENTHANDLER_HELP_MESSAGE_ARGUMENTS(SEC_BUILTINARGUMENTS_CLIENT, "<path>", SEC_BUILTINARGUMENTS_CLIENT_SHORT, "Specifies what client you want to run, does nothing for standalone builds\n")
           BA_ARGUMENTHANDLER_HELP_MESSAGE(SEC_BUILTINARGUMENTS_SERVER, SEC_BUILTINARGUMENTS_SERVER_SHORT, "Starts the client as a server instance\n")
           BA_ARGUMENTHANDLER_HELP_MESSAGE(SEC_BUILTINARGUMENTS_NO_STRICT, SEC_BUILTINARGUMENTS_NO_STRICT_SHORT, "Don't crash the client when an API error occurs\n")
           BA_ARGUMENTHANDLER_HELP_MESSAGE_NO_SHORT_ARGUMENTS(SEC_BUILTINARGUMENTS_WIDTH, "<width>", "Changes the width of the window\n")
           BA_ARGUMENTHANDLER_HELP_MESSAGE_NO_SHORT_ARGUMENTS(SEC_BUILTINARGUMENTS_HEIGHT, "<height>", "Changes the height of the window\n")
           BA_ARGUMENTHANDLER_HELP_MESSAGE_NO_SHORT_ARGUMENTS(SEC_BUILTINARGUMENTS_RENDERER, "<opengl/vulkan/text/software>", "Changes the default rendering system\n")
           BA_ARGUMENTHANDLER_HELP_MESSAGE_NO_SHORT_ARGUMENTS(SEC_BUILTINARGUMENTS_MONITOR, "<id>", "Create the window on a different display\n")
           BA_ARGUMENTHANDLER_HELP_MESSAGE(SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING, SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING_SHORT, "Don't render the UI\n")
           BA_ARGUMENTHANDLER_HELP_MESSAGE_NO_SHORT_ARGUMENTS(SEC_BUILTINARGUMENTS_EXIT, "<code>", "Exit the engine after initializing\n")
           BA_ARGUMENTHANDLER_HELP_MESSAGE_NO_SHORT(SEC_BUILTINARGUMENTS_CONSOLE, "Start with the console opened\n")
           BA_ARGUMENTHANDLER_HELP_MESSAGE(SEC_BUILTINARGUMENTS_VERSION, SEC_BUILTINARGUMENTS_VERSION_SHORT, "Show the version of the launcher/engine\n")
           #ifdef BA_ALLOW_DEBUG_LOGS
           BA_ARGUMENTHANDLER_HELP_MESSAGE(SEC_BUILTINARGUMENTS_DONT_PRINT_STRICT_CHECKS, SEC_BUILTINARGUMENTS_DONT_PRINT_STRICT_CHECKS_SHORT, "Do not log strict checks, does nothing if log level is not trace\n")
           BA_ARGUMENTHANDLER_HELP_MESSAGE(SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOCATION, SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOCATION_SHORT, "Do not log when the engine allocates memory, does nothing if log level is not trace\n")
           #endif
           BA_ARGUMENTHANDLER_HELP_MESSAGE_ARGUMENTS(SEC_BUILTINARGUMENTS_ENGINE, "<engine binary>", SEC_BUILTINARGUMENTS_ENGINE_SHORT, "Use a custom engine binary\n")
           #ifdef BA_ALLOW_DEBUG_LOGS
           BA_ARGUMENTHANDLER_HELP_MESSAGE(SEC_BUILTINARGUMENTS_DONT_PRINT_COMMAND_REGISTER, SEC_BUILTINARGUMENTS_DONT_PRINT_COMMAND_REGISTER_SHORT, "Do not log when a command gets registered, does nothing if log level is not trace\n")
           #endif
           BA_ARGUMENTHANDLER_HELP_MESSAGE_ARGUMENTS(SEC_BUILTINARGUMENTS_PORT, "<port>", SEC_BUILTINARGUMENTS_PORT_SHORT, "Custom server port, useless outside server mode\n")
           BA_ARGUMENTHANDLER_HELP_MESSAGE_ARGUMENTS(SEC_BUILTINARGUMENTS_MAX_PLAYERS, "<amount>", SEC_BUILTINARGUMENTS_MAX_PLAYERS_SHORT, "Tells how many players can connect, useless outside server mode\n");

#undef BA_LAUNCHER_HELP_LOGGER_EXTRA_ARGUMENTS
#undef SEC_LAUNCHER_HELP_MESSAGE_NO_SHORT
#undef SEC_LAUNCHER_HELP_MESSAGE
}

void SEC_Launcher_InitializeEngine(SEC_Launcher_Configuration* configuration) {
    configuration->unionVariables.data.engineBinary = BA_PLATFORMSPECIFIC_GET_BINARY(SEC_Paths_GetEngineBinaryPath(), RTLD_NOW);

    if (configuration->unionVariables.data.engineBinary == NULL) {
        configuration->code = SEC_LAUNCHER_ERROR_CODE_BINARY;
        configuration->unionVariables.errorReason.isEngine = BA_BOOLEAN_TRUE;

        BA_PLATFORMSPECIFIC_GET_ERROR(configuration->unionVariables.errorReason.errorMessage);
        return;
    }

    BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(SEC_Launcher_EngineStart, configuration->unionVariables.data.Start, BA_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.engineBinary, "BE_EntryPoint_StartBaconEngine"));

    if (configuration->unionVariables.data.Start != NULL)
        return;

    configuration->code = SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL;
    configuration->unionVariables.errorReason.isEngine = BA_BOOLEAN_TRUE;

    BA_PLATFORMSPECIFIC_GET_ERROR(configuration->unionVariables.errorReason.errorMessage);
}

void SEC_Launcher_InitializeClient(SEC_Launcher_Configuration* configuration) {
    configuration->unionVariables.data.clientBinary = BA_PLATFORMSPECIFIC_GET_BINARY(SEC_Paths_GetClientBinaryPath(), RTLD_NOW);

    if (configuration->unionVariables.data.clientBinary == NULL) {
        configuration->code = SEC_LAUNCHER_ERROR_CODE_BINARY;
        configuration->unionVariables.errorReason.isEngine = BA_BOOLEAN_FALSE;

        BA_PLATFORMSPECIFIC_GET_ERROR(configuration->unionVariables.errorReason.errorMessage);
        return;
    }
    
    const char* (*getName)(void);

    BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(const char* (*)(void), getName, BA_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.clientBinary, "I_EntryPoint_GetName"));

    configuration->unionVariables.data.clientName = getName != NULL ? getName() : "";
}

SEC_Launcher_StartEngineResults SEC_Launcher_StartEngine(const SEC_Launcher_Configuration* configuration) {
    SEC_Launcher_ClientInitialize clientInitialize;

    BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(SEC_Launcher_ClientInitialize, clientInitialize,
                                                 BA_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.clientBinary, "I_EntryPoint_InitializeWrapper"));
    
    if (clientInitialize == NULL) {
        SEC_Launcher_StartEngineResults results = BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(SEC_Launcher_StartEngineResults, BA_BOOLEAN_FALSE);

        BA_PLATFORMSPECIFIC_GET_ERROR(results.unionVariables.errorMessage);
        
        return results;
    }

    SEC_Launcher_ClientStart clientStart;
    SEC_Launcher_ClientShutdown clientShutdown;
    SEC_Launcher_ClientSupportsServer clientSupportsServer;
    SEC_Launcher_ClientGetName clientGetName;
    SEC_Launcher_ClientGetEngineVersion clientGetEngineVersion;
    SEC_Launcher_ClientGetEngineName clientGetEngineName;

    BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(SEC_Launcher_ClientStart, clientStart, BA_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.clientBinary, "I_EntryPoint_Start"));
    BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(SEC_Launcher_ClientShutdown, clientShutdown, BA_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.clientBinary, "I_EntryPoint_Shutdown"));
    BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(SEC_Launcher_ClientSupportsServer, clientSupportsServer, BA_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.clientBinary, "I_EntryPoint_SupportsServer"));
    BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(SEC_Launcher_ClientGetName, clientGetName, BA_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.clientBinary, "I_EntryPoint_GetName"));
    BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(SEC_Launcher_ClientGetEngineVersion, clientGetEngineVersion, BA_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.clientBinary, "I_EntryPoint_GetEngineVersion"));
    BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(SEC_Launcher_ClientGetEngineName, clientGetEngineName, BA_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.clientBinary, "I_EntryPoint_GetEngineName"));
    
    SEC_Launcher_EngineDetails details = BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(SEC_Launcher_EngineDetails,
        BA_ArgumentHandler_GetCount(),
        BA_ArgumentHandler_GetVector(),
        SEC_Paths_GetLauncherDirectory(),
        SEC_Paths_GetEngineDirectory(),
        SEC_Paths_GetClientDirectory(),
        configuration->unionVariables.data.engineBinary,
        clientInitialize,
        clientStart,
        clientShutdown,
        clientSupportsServer,
        clientGetName,
        clientGetEngineVersion,
        clientGetEngineName);
    
    SEC_Launcher_StartEngineResults results = BA_CPLUSPLUS_SUPPORT_CREATE_STRUCT(SEC_Launcher_StartEngineResults, BA_BOOLEAN_TRUE);
    
    results.unionVariables.returnCode = configuration->unionVariables.data.Start(&details);
    return results;
}

void SEC_Launcher_SetEnginePath(void) {
    BA_ArgumentHandler_ShortResults results;

    if (BA_ArgumentHandler_GetInformationWithShort(SEC_BUILTINARGUMENTS_ENGINE, SEC_BUILTINARGUMENTS_ENGINE_SHORT,
                                                   BA_BOOLEAN_FALSE, &results) != 0)
        SEC_Paths_SetEnginePath(*results.value);
    else
        SEC_Paths_SetEnginePath("Engines/Official");
}

void SEC_Launcher_SetLauncherPath(void) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    static char directory[PATH_MAX];
    
    getcwd(directory, PATH_MAX);
#else
    static char directory[MAX_PATH];

    GetCurrentDirectory(MAX_PATH, directory);
#endif

    SEC_Paths_SetLauncherPath(directory);
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
