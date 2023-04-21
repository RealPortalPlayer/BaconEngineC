// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <errno.h>

#include "SharedEngineCode/Launcher.h"
#include "SharedEngineCode/Internal/CppSupport.h"
#include "SharedEngineCode/BuiltInArguments.h"

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <dlfcn.h>
#   include <unistd.h>
#   define CHDIR(dir) chdir(dir)
#   define GET_BINARY(name, options) dlopen(name, options)
#   define GET_ADDRESS(binary, name) dlsym(binary, name)
#   define SET_ERROR() configuration->unionVariables.errorMessage = dlerror()
#elif SEC_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#   include <direct.h>
#   define CHDIR(dir) _chdir(dir)
#   define GET_BINARY(name, options) LoadLibrary(name)
#   define GET_ADDRESS(binary, name) GetProcAddress(binary, name)
#   define SET_ERROR() do { \
    DWORD id = GetLastError(); \
    LPSTR message = NULL;   \
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&message, 0, NULL); \
    configuration->unionVariables.errorMessage = message; \
} while (0)
#   define BINARY_EXTENSION ".dll"
#endif

#if SEC_OPERATINGSYSTEM_APPLE
#   define BINARY_EXTENSION ".dylib"
#elif SEC_OPERATINGSYSTEM_LINUX || SEC_OPERATINGSYSTEM_UNIX || SEC_OPERATINGSYSTEM_SERENITY
#   define BINARY_EXTENSION ".so"
#endif

SEC_CPP_SUPPORT_GUARD_START()
void SEC_Launcher_CreateConfiguration(SEC_Launcher_Configuration* configuration, const char* path) {
    if (CHDIR(path) != 0) {
        configuration->code = SEC_LAUNCHER_ERROR_CODE_CHDIR;
        configuration->unionVariables.errorMessage = strerror(errno);
        return;
    }

    configuration->unionVariables.data.clientBinary = GET_BINARY("./binary" BINARY_EXTENSION, RTLD_NOW);

    if (configuration->unionVariables.data.clientBinary == NULL) {
        configuration->code = SEC_LAUNCHER_ERROR_CODE_BINARY;

        SET_ERROR();
        return;
    }

    {
        const char*
        (*name)(void) = (const char*(*)(void)) GET_ADDRESS(configuration->unionVariables.data.clientBinary, "BE_EntryPoint_GetClientName");

        if (name == NULL) {
            configuration->code = SEC_LAUNCHER_ERROR_CODE_NAME_NULL;

            SET_ERROR();
            return;
        }

        configuration->unionVariables.data.clientName = name();
    }

    configuration->unionVariables.data.Start = (int (*)(int, char**)) GET_ADDRESS(configuration->unionVariables.data.clientBinary,
                                                                                  "BE_EntryPoint_InitializeDynamicLibrary");

    if (configuration->unionVariables.data.Start == NULL) {
        configuration->code = SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL;

        SET_ERROR();
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
           SEC_BUILTINARGUMENTS_LOG_LEVEL " <null/trace/trc/debug/dbg/warn/wrn/error/err/fatal/ftl> (" SEC_BUILTINARGUMENTS_LOG_LEVEL_SHORT "): Sets the current log level\n"
           SEC_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS " (" SEC_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS_SHORT "): Prevent the client from changing the log level\n"
           SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING " (" SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING_SHORT "): Don't render the UI\n"
           SEC_BUILTINARGUMENTS_EXIT " <code>: Exit the engine after initializing\n"
           SEC_BUILTINARGUMENTS_CONSOLE ": Start with the console opened\n"
           SEC_BUILTINARGUMENTS_VERSION " (" SEC_BUILTINARGUMENTS_VERSION_SHORT "): Show the version of the launcher/engine\n"
           SEC_BUILTINARGUMENTS_ALWAYS_USE_STDOUT " (" SEC_BUILTINARGUMENTS_ALWAYS_USE_STDOUT_SHORT "): Always use STDOUT, even for errors\n"
           SEC_BUILTINARGUMENTS_DONT_PRINT_ASSERT_CHECKS " (" SEC_BUILTINARGUMENTS_DONT_PRINT_ASSERT_CHECKS_SHORT "): Do not log assert checks, does nothing if log level is not trace\n"
           SEC_BUILTINARGUMENTS_DONT_PRINT_STRICT_CHECKS " (" SEC_BUILTINARGUMENTS_DONT_PRINT_STRICT_CHECKS_SHORT "): Do not log strict checks, does nothing if log level is not trace\n"
           SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOC " (" SEC_BUILTINARGUMENTS_DONT_PRINT_ENGINE_MEMORY_ALLOC_SHORT "): Do not log when the engine allocates memory, does nothing if log level is not trace\n"
/*#if SEC_OPERATINGSYSTEM_WINDOWS // TODO: Reimplement WinMain, but as a separate executable
           SEC_BUILTINARGUMENTS_SHOW_TERMINAL ": Shows a terminal window, does nothing for non-Windows\n"
#endif*/
           SEC_BUILTINARGUMENTS_DISABLE_LOG_HEADER " (" SEC_BUILTINARGUMENTS_DISABLE_LOG_HEADER_SHORT "): Do not log the log level header";
}
SEC_CPP_SUPPORT_GUARD_END()
