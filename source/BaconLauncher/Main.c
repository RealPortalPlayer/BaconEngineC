#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <SharedEngineCode/Launcher.h>

#if SEC_OS_POSIX_COMPLIANT
#   include <unistd.h>
#elif SEC_OS_WINDOWS
#   include <Windows.h>
#endif

#ifdef BACON_ENGINE_LAUNCHER
char* BE_GetClientName(void);
int BE_StartBaconEngine(int argc, char** argv);
#else
#   include <stddef.h>
#   if SEC_OS_POSIX_COMPLIANT
#       include <dlfcn.h>
#   endif
#endif

#ifdef BACON_ENGINE_LAUNCHER
int CallLauncherMain(int argc, char** argv) {
#else
int main(int argc, char** argv) {
#endif
    SEC_InitializeArgumentHandler(argc, argv);

    if (SEC_GetArgumentIndex("--enable-debug-logs") != -1 || SEC_GetArgumentIndex("-edl") != -1)
        SEC_SetLogLevel(SEC_LOG_LEVEL_DEBUG);

    if (SEC_GetArgumentIndex("--enable-trace-logs") != -1 || SEC_GetArgumentIndex("-etl") != -1)
        SEC_SetLogLevel(SEC_LOG_LEVEL_TRACE);

    SEC_LOG_TRACE("Built: %s", __TIMESTAMP__);

    if (SEC_GetArgumentIndex("--help") != -1) {
        SEC_LOG_INFO("Arguments:\n"
                 "--help: Shows information about each argument\n"
#ifndef BACON_ENGINE_LAUNCHER
                 "--client <path> (-c): Specifies what client you want to run\n"
#endif
                 "--server (-s): Starts the client as a server instance\n"
                 "--no-strict (-ns): Don't crash the client when an API error occurs.\n"
                 "--enable-debug-logs (-edl): Enables debugging logs\n"
                 "--enable-trace-logs (-etl): Enables tracing logs. This will also enable debug logs, too\n"
                 "--dont-parse <argument> (--): Do not parse argument's beyond this point\n"
#ifndef BACON_ENGINE_DISABLE_SDL
                 "--width <width>: Changes the width of the window\n"
                 "--height <height>: Changes the height of the window\n"
                 "--renderer <renderer>: Changes the default rendering system\n"
                 "--software: Use software mode to render instead of your GPU"
#endif
);
        return 0;
    }

    // TODO: OS_WINDOWS
#if SEC_OS_POSIX_COMPLIANT
    if (getuid() == 0)
        SEC_LOG_WARN("You're running as root! If a client says you require to be root, then it's probably a virus");
#endif

#ifndef BACON_ENGINE_LAUNCHER
    const char* clientPath = SEC_GetArgumentValue("--client");

    if (clientPath == NULL)
        clientPath = SEC_GetArgumentValue("-c");

    if (clientPath == NULL) {
        SEC_LOG_WARN("You didn't specify what client you wanted to open, defaulting to 'Client'");

        clientPath = "./Client";
    }
#endif

    SEC_LOG_INFO("Getting configuration information");

#ifndef BACON_ENGINE_LAUNCHER
    SEC_LauncherConfiguration configuration = {
        .code = SEC_LAUNCHER_ERROR_CODE_NULL
    };

    SEC_CreateLauncherConfiguration(&configuration, clientPath);

    if (configuration.code != SEC_LAUNCHER_ERROR_CODE_NULL) {
        switch (configuration.code) {
            case SEC_LAUNCHER_ERROR_CODE_BINARY:
#if OS_POSIX_COMPLIANT
                LOG_FATAL("Failed to load the binary: %s", configuration.errorMessage);
#elif OS_WINDOWS
                LOG_FATAL("Failed to load the binary: %i", configuration.code);
#endif
                return 1;

            case SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL:
            case SEC_LAUNCHER_ERROR_CODE_NAME_NULL:
#if SEC_OS_POSIX_COMPLIANT
                SEC_LOG_FATAL("Failed to get important methods: %s", configuration.errorMessage);
#elif SEC_OS_WINDOWS
                SEC_LOG_FATAL("Failed to get important methods: %i", configuration.code);
#endif
                return 1;

            default:
                SEC_LOG_FATAL("Unknown error: %i", configuration.code);
                return 1;
        }
    }

    SEC_LOG_INFO("Ready, starting '%s'", configuration.clientName);
    SEC_LOG_TRACE("Entering client code");

    int returnValue = configuration.Start(argc, argv);

    SEC_LOG_TRACE("Returned back to launcher");
#else
    SEC_LOG_INFO("Skipping, due to this being a standalone launcher");
    SEC_LOG_INFO("Ready, starting '%s'", BE_GetClientName());

    int returnValue = BE_StartBaconEngine(argc, argv);
#endif

#ifndef BACON_ENGINE_LAUNCHER
    SEC_LOG_TRACE("Freeing binaries");

#   if SEC_OS_POSIX_COMPLIANT
    dlclose(configuration.clientBinary);
#   elif SEC_OS_WINDOWS
    FreeLibrary(configuration.clientBinary);
#   endif
#endif

    SEC_LOG_INFO("Goodbye");

    return returnValue;
}