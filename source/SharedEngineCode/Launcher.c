// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <errno.h>

#include "SharedEngineCode/Launcher.h"
#include "SharedEngineCode/Internal/CPlusPlusSupport.h"
#include "SharedEngineCode/BuiltInArguments.h"
#include "SharedEngineCode/Internal/PlatformSpecific.h"
#include "SharedEngineCode/ArgumentHandler.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
void SEC_Launcher_CreateConfiguration(SEC_Launcher_Configuration* configuration, const char* path) {
    {
        SEC_ArgumentHandler_ShortResults results;

        if (SEC_ArgumentHandler_GetInformationWithShort(SEC_BUILTINARGUMENTS_ENGINE, SEC_BUILTINARGUMENTS_ENGINE_SHORT,
                                                        SEC_BOOLEAN_FALSE, &results) != 0)
            configuration->unionVariables.data.engineBinary = SEC_PLATFORMSPECIFIC_GET_BINARY(*results.value, RTLD_NOW);
        else
            configuration->unionVariables.data.engineBinary = SEC_PLATFORMSPECIFIC_GET_BINARY("./BaconEngine" SEC_PLATFORMSPECIFIC_BINARY_EXTENSION, RTLD_NOW);
    }

    if (configuration->unionVariables.data.engineBinary == NULL) {
        configuration->code = SEC_LAUNCHER_ERROR_CODE_BINARY;
        configuration->unionVariables.errorReason.isEngine = SEC_BOOLEAN_TRUE;

        SEC_PLATFORMSPECIFIC_GET_ERROR(configuration->unionVariables.errorReason.errorMessage);
        return;
    }

    if (SEC_PLATFORMSPECIFIC_CHANGE_DIRECTORY(path) != 0) {
        configuration->code = SEC_LAUNCHER_ERROR_CODE_CHDIR;
        configuration->unionVariables.errorReason.isEngine = SEC_BOOLEAN_FALSE;
        configuration->unionVariables.errorReason.errorMessage = strerror(errno);
        return;
    }

    configuration->unionVariables.data.clientBinary = SEC_PLATFORMSPECIFIC_GET_BINARY("./binary" SEC_PLATFORMSPECIFIC_BINARY_EXTENSION, RTLD_NOW);

    if (configuration->unionVariables.data.clientBinary == NULL) {
        configuration->code = SEC_LAUNCHER_ERROR_CODE_BINARY;
        configuration->unionVariables.errorReason.isEngine = SEC_BOOLEAN_FALSE;

        SEC_PLATFORMSPECIFIC_GET_ERROR(configuration->unionVariables.errorReason.errorMessage);
        return;
    }

    {
        const char*
        (*name)(void) = (const char* (*)(void)) SEC_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.clientBinary, "I_EntryPoint_GetName");

        configuration->unionVariables.data.clientName = name != NULL ? name() : "";
    }

    configuration->unionVariables.data.Start = (int (*)(void*, void*, int, char**)) SEC_PLATFORMSPECIFIC_GET_ADDRESS(configuration->unionVariables.data.engineBinary,
                                                                                                                     "BE_EntryPoint_StartBaconEngine");

    if (configuration->unionVariables.data.Start == NULL) {
        configuration->code = SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL;
        configuration->unionVariables.errorReason.isEngine = SEC_BOOLEAN_TRUE;

        SEC_PLATFORMSPECIFIC_GET_ERROR(configuration->unionVariables.errorReason.errorMessage);
        return;
    }

    configuration->code = SEC_LAUNCHER_ERROR_CODE_NULL;
}

const char* SEC_Launcher_GetDefaultHelpList(void) {
    return SEC_BUILTINARGUMENTS_HELP " (" SEC_BUILTINARGUMENTS_HELP_SHORT "): Shows information about each argument\n"
#ifndef BACON_ENGINE_LAUNCHER
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
           SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOC " (" SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOC_SHORT "): Do not log when the engine allocates memory, does nothing if log level is not trace\n"
#endif
           SEC_BUILTINARGUMENTS_DISABLE_LOG_HEADER " (" SEC_BUILTINARGUMENTS_DISABLE_LOG_HEADER_SHORT "): Do not log the log level header\n"
           SEC_BUILTINARGUMENTS_ENGINE " <engine binary> (" SEC_BUILTINARGUMENTS_ENGINE_SHORT "): Use a custom engine binary";
}
SEC_CPLUSPLUS_SUPPORT_GUARD_END()
