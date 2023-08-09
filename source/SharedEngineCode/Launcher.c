// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "SharedEngineCode/Launcher.h"
#include "SharedEngineCode/Internal/CPlusPlusSupport.h"
#include "SharedEngineCode/BuiltInArguments.h"
#include "SharedEngineCode/Internal/PlatformSpecific.h"
#include "SharedEngineCode/ArgumentHandler.h"
#include "SharedEngineCode/ArgumentHandler.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
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
    return SEC_BUILTINARGUMENTS_HELP " (" SEC_BUILTINARGUMENTS_HELP_SHORT "): Shows information about each argument\n"
#ifndef BE_STANDALONE_CLIENT
           SEC_BUILTINARGUMENTS_CLIENT " <path> (" SEC_BUILTINARGUMENTS_CLIENT_SHORT "): Specifies what client you want to run\n"
#endif
           SEC_BUILTINARGUMENTS_SERVER " (" SEC_BUILTINARGUMENTS_SERVER_SHORT "): Starts the client as a server instance\n"
           SEC_BUILTINARGUMENTS_NO_STRICT " (" SEC_BUILTINARGUMENTS_NO_STRICT_SHORT "): Don't crash the client when an API error occurs\n"
           SEC_BUILTINARGUMENTS_DONT_PARSE " <argument>: Do not parse arguments beyond this point\n"
           SEC_BUILTINARGUMENTS_WIDTH " <width>: Changes the width of the window\n"
           SEC_BUILTINARGUMENTS_HEIGHT " <height>: Changes the height of the window\n"
           SEC_BUILTINARGUMENTS_RENDERER " <opengl/vulkan/text/software>: Changes the default rendering system\n"
           SEC_BUILTINARGUMENTS_MONITOR " <id>: Create the window on a different display\n"
           SEC_BUILTINARGUMENTS_DISABLE_ANSI_COLORING " (" SEC_BUILTINARGUMENTS_DISABLE_ANSI_COLORING_SHORT "): Disable log colors\n"
           SEC_BUILTINARGUMENTS_LOG_LEVEL " <null"
#ifdef BE_ALLOW_DEBUG_LOGS
           "/trace/trc/debug/dbg"
#endif
           "/warn/wrn/error/err/fatal/ftl> (" SEC_BUILTINARGUMENTS_LOG_LEVEL_SHORT "): Sets the current log level\n"
           SEC_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS " (" SEC_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS_SHORT "): Prevent the client from changing the log level\n"
           SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING " (" SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING_SHORT "): Don't render the UI\n"
           SEC_BUILTINARGUMENTS_EXIT " <code>: Exit the engine after initializing\n"
           SEC_BUILTINARGUMENTS_CONSOLE ": Start with the console opened\n"
           SEC_BUILTINARGUMENTS_VERSION " (" SEC_BUILTINARGUMENTS_VERSION_SHORT "): Show the version of the launcher/engine\n"
           SEC_BUILTINARGUMENTS_ALWAYS_USE_STDOUT " (" SEC_BUILTINARGUMENTS_ALWAYS_USE_STDOUT_SHORT "): Always use STDOUT, even for errors\n"
#ifdef BE_ALLOW_DEBUG_LOGS
           SEC_BUILTINARGUMENTS_DONT_PRINT_ASSERT_CHECKS " (" SEC_BUILTINARGUMENTS_DONT_PRINT_ASSERT_CHECKS_SHORT "): Do not log assert checks, does nothing if log level is not trace\n"
           SEC_BUILTINARGUMENTS_DONT_PRINT_STRICT_CHECKS " (" SEC_BUILTINARGUMENTS_DONT_PRINT_STRICT_CHECKS_SHORT "): Do not log strict checks, does nothing if log level is not trace\n"
           SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOCATION " (" SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOCATION_SHORT "): Do not log when the engine allocates memory, does nothing if log level is not trace\n"
#endif
           SEC_BUILTINARGUMENTS_DISABLE_LOG_HEADER " (" SEC_BUILTINARGUMENTS_DISABLE_LOG_HEADER_SHORT "): Do not log the log level header\n"
           SEC_BUILTINARGUMENTS_ENGINE " <engine binary> (" SEC_BUILTINARGUMENTS_ENGINE_SHORT "): Use a custom engine binary"
#ifdef BE_ALLOW_DEBUG_LOGS
           "\n"
           SEC_BUILTINARGUMENTS_DONT_PRINT_COMMAND_REGISTER " (" SEC_BUILTINARGUMENTS_DONT_PRINT_COMMAND_REGISTER_SHORT "): Do not log when a command gets registered, does nothing if log level is not trace"
#endif
            ;
}

void SEC_Launcher_InitializeEngine(SEC_Launcher_Configuration* configuration) {
    configuration->unionVariables.data.engineBinary = SEC_PLATFORMSPECIFIC_GET_BINARY(SEC_Paths_GetEngineBinaryPath(), RTLD_NOW);

    if (configuration->unionVariables.data.engineBinary == NULL) {
        configuration->code = SEC_LAUNCHER_ERROR_CODE_BINARY;
        configuration->unionVariables.errorReason.isEngine = SEC_BOOLEAN_TRUE;

        SEC_PLATFORMSPECIFIC_GET_ERROR(configuration->unionVariables.errorReason.errorMessage);
        return;
    }

    SEC_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(SEC_Launcher_EngineStart, configuration->unionVariables.data.Start, SEC_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.engineBinary, "BE_EntryPoint_StartBaconEngine"));

    if (configuration->unionVariables.data.Start != NULL)
        return;

    configuration->code = SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL;
    configuration->unionVariables.errorReason.isEngine = SEC_BOOLEAN_TRUE;

    SEC_PLATFORMSPECIFIC_GET_ERROR(configuration->unionVariables.errorReason.errorMessage);
}


void SEC_Launcher_InitializeClient(SEC_Launcher_Configuration* configuration) {
    configuration->unionVariables.data.clientBinary = SEC_PLATFORMSPECIFIC_GET_BINARY(SEC_Paths_GetClientBinaryPath(), RTLD_NOW);

    if (configuration->unionVariables.data.clientBinary == NULL) {
        configuration->code = SEC_LAUNCHER_ERROR_CODE_BINARY;
        configuration->unionVariables.errorReason.isEngine = SEC_BOOLEAN_FALSE;

        SEC_PLATFORMSPECIFIC_GET_ERROR(configuration->unionVariables.errorReason.errorMessage);
        return;
    }
    
    const char* (*getName)(void);

    SEC_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(const char* (*)(void), getName, SEC_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.clientBinary, "I_EntryPoint_GetName"));

    configuration->unionVariables.data.clientName = getName != NULL ? getName() : "";
}

SEC_Launcher_StartEngineResults SEC_Launcher_StartEngine(const SEC_Launcher_Configuration* configuration) {
    SEC_Launcher_ClientInitialize clientInitialize;

    SEC_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(SEC_Launcher_ClientInitialize, clientInitialize,
                                                  SEC_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.clientBinary, "I_EntryPoint_InitializeWrapper"));
    
    if (clientInitialize == NULL) {
        SEC_Launcher_StartEngineResults results = SEC_CPLUSPLUS_SUPPORT_CREATE_STRUCT(SEC_Launcher_StartEngineResults, SEC_BOOLEAN_FALSE);

        SEC_PLATFORMSPECIFIC_GET_ERROR(results.unionVariables.errorMessage);
        
        return results;
    }

    SEC_Launcher_ClientStart clientStart;
    SEC_Launcher_ClientShutdown clientShutdown;
    SEC_Launcher_ClientSupportsServer clientSupportsServer;
    SEC_Launcher_ClientGetName clientGetName;
    SEC_Launcher_ClientGetEngineVersion clientGetEngineVersion;

    SEC_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(SEC_Launcher_ClientStart, clientStart, SEC_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.clientBinary, "I_EntryPoint_Start"));
    SEC_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(SEC_Launcher_ClientShutdown, clientShutdown, SEC_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.clientBinary, "I_EntryPoint_Shutdown"));
    SEC_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(SEC_Launcher_ClientSupportsServer, clientSupportsServer, SEC_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.clientBinary, "I_EntryPoint_SupportsServer"));
    SEC_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(SEC_Launcher_ClientGetName, clientGetName, SEC_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.clientBinary, "I_EntryPoint_GetName"));
    SEC_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(SEC_Launcher_ClientGetEngineVersion, clientGetEngineVersion, SEC_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.clientBinary, "I_EntryPoint_GetEngineVersion"));

    SEC_Launcher_EngineDetails details = SEC_CPLUSPLUS_SUPPORT_CREATE_STRUCT(SEC_Launcher_EngineDetails,
                                             SEC_ArgumentHandler_GetCount(),
                                             SEC_ArgumentHandler_GetVector(),
                                             SEC_Paths_GetLauncherDirectory(),
                                             SEC_Paths_GetEngineDirectory(),
                                             SEC_Paths_GetClientDirectory(),
                                             configuration->unionVariables.data.engineBinary,
                                             clientInitialize,
                                             clientStart,
                                             clientShutdown,
                                             clientSupportsServer,
                                             clientGetName,
                                             clientGetEngineVersion
                                         );
    
    SEC_Launcher_StartEngineResults results = SEC_CPLUSPLUS_SUPPORT_CREATE_STRUCT(SEC_Launcher_StartEngineResults, SEC_BOOLEAN_TRUE);
    
    results.unionVariables.returnCode = configuration->unionVariables.data.Start(&details);
    return results;
}

void SEC_Launcher_SetEnginePath(void) {
    SEC_ArgumentHandler_ShortResults results;

    if (SEC_ArgumentHandler_GetInformationWithShort(SEC_BUILTINARGUMENTS_ENGINE, SEC_BUILTINARGUMENTS_ENGINE_SHORT,
                                                    SEC_BOOLEAN_FALSE, &results) != 0)
        SEC_Paths_SetEnginePath(*results.value);
    else
        SEC_Paths_SetEnginePath("Engines/Official");
}

void SEC_Launcher_SetLauncherPath(void) {
#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
    static char directory[PATH_MAX];
    
    getcwd(directory, PATH_MAX);
#else
    static char directory[MAX_PATH];

    GetCurrentDirectory(MAX_PATH, directory);
#endif

    SEC_Paths_SetLauncherPath(directory);
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
