#include "SharedEngineCode/Launcher.h"

#include "SharedEngineCode/Internal/CppHeader.h"
#include "SharedEngineCode/BuiltInArguments.h"

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <dlfcn.h>
#   include <unistd.h>
#   define CHDIR(dir) chdir(dir)
#   define GET_BINARY(name, options) dlopen(name, options)
#   define GET_ADDRESS(binary, name) dlsym(binary, name)
#   define SET_ERROR() configuration->errorMessage = dlerror()
#   define BINARY_EXTENSION ".dylib"
#elif SEC_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#   include <direct.h>
#   define CHDIR(dir) _chdir(dir)
#   define GET_BINARY(name, options) LoadLibrary(name)
#   define GET_ADDRESS(binary, name) GetProcAddress(binary, name)
#   define SET_ERROR() configuration->errorMessage = NULL
#   define BINARY_EXTENSION ".dll"
#endif

SEC_CPP_GUARD_START()
    void SEC_Launcher_CreateConfiguration(SEC_Launcher_Configuration* configuration, const char* path) {
        CHDIR(path);

        configuration->clientBinary = GET_BINARY("binary" BINARY_EXTENSION, RTLD_NOW);

        if (configuration->clientBinary == NULL) {
            configuration->code = SEC_LAUNCHER_ERROR_CODE_BINARY;

            SET_ERROR();
            return;
        }

        {
            const char* (*name)(void) = (const char* (*)(void)) GET_ADDRESS(configuration->clientBinary, "BE_EntryPoint_GetClientName");

            if (name == NULL) {
                configuration->code = SEC_LAUNCHER_ERROR_CODE_NAME_NULL;

                SET_ERROR();
                return;
            }

            configuration->clientName = name();
        }

        configuration->Start = (int (*)(int, char**)) GET_ADDRESS(configuration->clientBinary, "BE_EntryPoint_InitializeDynamicLibrary");

        if (configuration->Start == NULL) {
            configuration->code = SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL;
            SET_ERROR();
            return;
        }

        configuration->code = SEC_LAUNCHER_ERROR_CODE_NULL;
    }

    const char* SEC_Launcher_GetDefaultHelpList(void) {
        return SEC_BUILTINARGUMENTS_HELP ": Shows information about each argument\n"
#ifndef BACON_ENGINE_LAUNCHER
               SEC_BUILTINARGUMENTS_CLIENT " <path> (" SEC_BUILTINARGUMENTS_CLIENT_SHORT "): Specifies what client you want to run\n"
#endif
               SEC_BUILTINARGUMENTS_SERVER " (" SEC_BUILTINARGUMENTS_SERVER_SHORT "): Starts the client as a server instance\n"
               SEC_BUILTINARGUMENTS_NO_STRICT " (" SEC_BUILTINARGUMENTS_NO_STRICT_SHORT "): Don't crash the client when an API error occurs.\n"
               SEC_BUILTINARGUMENTS_DONT_PARSE " <argument>: Do not parse argument's beyond this point\n"
#ifndef BACON_ENGINE_DISABLE_SDL
               SEC_BUILTINARGUMENTS_WIDTH " <width>: Changes the width of the window\n"
               SEC_BUILTINARGUMENTS_HEIGHT " <height>: Changes the height of the window\n"
               SEC_BUILTINARGUMENTS_RENDERER " <opengl/vulkan/text>: Changes the default rendering system\n"
               SEC_BUILTINARGUMENTS_SOFTWARE ": Use your CPU to render instead of your GPU\n"
               SEC_BUILTINARGUMENTS_MONITOR " <id>: Create the window on a different display\n"
#endif
               SEC_BUILTINARGUMENTS_DISABLE_ANSI_COLORING " (" SEC_BUILTINARGUMENTS_DISABLE_ANSI_COLORING_SHORT "): Disable log colors\n"
               SEC_BUILTINARGUMENTS_LOG_LEVEL " <null/trace/trc/debug/dbg/warn/wrn/error/err/fatal/ftl> (" SEC_BUILTINARGUMENTS_LOG_LEVEL_SHORT "): Sets the current log level\n"
               SEC_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS " (" SEC_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS_SHORT "): Prevent the client from changing the log level\n"
               SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING " (" SEC_BUILTINARGUMENTS_DISABLE_UI_RENDERING_SHORT "): Don't render the UI\n"
               SEC_BUILTINARGUMENTS_EXIT " <code>: Exit the engine after initializing\n"
               SEC_BUILTINARGUMENTS_CONSOLE ": Start with the console opened";
    }
SEC_CPP_GUARD_END()
