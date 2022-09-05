#include "SharedEngineCode/Launcher.h"

#include "SharedEngineCode/Internal/CppHeader.h"

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

        configuration->Start = (int (*)(int, char**)) GET_ADDRESS(configuration->clientBinary, "BE_EntryPoint_StartBaconEngine");

        if (configuration->Start == NULL) {
            configuration->code = SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL;
            SET_ERROR();
            return;
        }

        configuration->code = SEC_LAUNCHER_ERROR_CODE_NULL;
    }

    const char* SEC_Launcher_GetDefaultHelpList(void) {
        return "--help: Shows information about each argument\n"
#ifndef BACON_ENGINE_LAUNCHER
               "--client <path> (-c): Specifies what client you want to run\n"
#endif
               "--server (-s): Starts the client as a server instance\n"
               "--no-strict (-ns): Don't crash the client when an API error occurs.\n"
               "--dont-parse <argument> (--): Do not parse argument's beyond this point\n"
#ifndef BACON_ENGINE_DISABLE_SDL
               "--width <width>: Changes the width of the window\n"
               "--height <height>: Changes the height of the window\n"
               "--renderer <opengl/vulkan/text>: Changes the default rendering system\n"
               "--software: Use your CPU to render instead of your GPU\n"
               "--monitor <id>: Create the window on a different display\n"
#endif
               "--disable-ansi-coloring (-dac): Disable log colors\n"
               "--dont-compress-dupe-logs (-dcdl): Don't compress logs, that are duplications of the last one, into one line\n"
               "--log-level <null/trace/trc/debug/dbg/warn/wrn/error/err/fatal/ftl> (-ll): Sets the current log level\n"
               "--dont-change-log-levels (-dcll): Prevent the client from changing the log level";
    }
SEC_CPP_GUARD_END()
