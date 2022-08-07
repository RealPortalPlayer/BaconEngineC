#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <stddef.h>
#include <SharedEngineCode/Launcher.h>
#include <SharedEngineCode/OperatingSystem.h>

#if OS_POSIX_COMPLIANT
#   include <unistd.h>
#   include <dlfcn.h>
#elif OS_WINDOWS
#   include <Windows.h>
#endif

int main(int argc, char* argv[]) {
    addedArgumentsCount = argc;
    argumentVector = argv;

    if (GetArgumentIndex("--enable-debug-logs") != -1 || GetArgumentIndex("-edl") != -1)
        currentLogLevel = LOG_LEVEL_DEBUG;

    if (GetArgumentIndex("--enable-trace-logs") != -1 || GetArgumentIndex("-etl") != -1)
        currentLogLevel = LOG_LEVEL_TRACE;

    LOG_TRACE("Built: %s", __TIMESTAMP__);

    if (GetArgumentIndex("--help") != -1) {
        LOG_INFO("Arguments:\n"
                 "--help: Shows information about each argument\n"
                 "--client <path> (-c): Specifies what client you want to run\n"
                 "--server (-s): Starts the client as a server instance\n"
                 "--no-strict (-ns): Don't crash the client when an API error occurs.\n"
                 "--enable-debug-logs (-edl): Enables debugging logs\n"
                 "--enable-trace-logs (-etl): Enables tracing logs. This will also enable debug logs, too\n"
                 "--dont-parse <argument> (--): Do not parse argument's beyond this point\n"
                 "--width <width>: Changes the width of the window\n"
                 "--height <height>: Changes the height of the window");
        return 0;
    }

    // TODO: OS_WINDOWS
#if OS_POSIX_COMPLIANT
    if (getuid() == 0)
        LOG_WARN("You're running as root! If a client says you require to be root, then it's probably a virus");
#endif

    const char* clientPath = GetArgumentValue("--client");

    if (clientPath == NULL)
        clientPath = GetArgumentValue("-c");

    if (clientPath == NULL) {
        LOG_WARN("You didn't specify what client you wanted to open, defaulting to 'Client'");

        clientPath = "./Client";
    }

    LOG_INFO("Getting configuration information");

    LauncherConfiguration configuration = {
        .code = LAUNCHER_ERROR_CODE_NULL
    };

    CreateLauncherConfiguration(&configuration, clientPath);

    if (configuration.code != LAUNCHER_ERROR_CODE_NULL) {
        switch (configuration.code) {
            case LAUNCHER_ERROR_CODE_BINARY:
                LOG_FATAL("Failed to load the binary: %s", configuration.errorMessage);
                return 1;

            case LAUNCHER_ERROR_CODE_ENTRY_NULL:
            case LAUNCHER_ERROR_CODE_NAME_NULL:
                LOG_FATAL("Failed to get important methods: %s", configuration.errorMessage);
                return 1;

            default:
                LOG_FATAL("Unknown error: %i", configuration.code);
                return 1;
        }
    }

    LOG_INFO("Ready, starting '%s'", configuration.clientName);

    int returnValue = configuration.Start(argc, argv);

#if OS_POSIX_COMPLIANT
    dlclose(configuration.clientBinary);
#elif OS_WINDOWS
    FreeLibrary(configuration.clientBinary);
#endif
    LOG_INFO("Goodbye");

    return returnValue;
}
