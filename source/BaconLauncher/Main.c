#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <stddef.h>
#include <SharedEngineCode/Launcher.h>
#include <stdlib.h>
#include <unistd.h>
#include <SharedEngineCode/OperatingSystem.h>

int main(int argc, char* argv[]) {
    argumentCount = argc;
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
                 "--strict: Crash the client if there is any API error\n"
                 "--enable-debug-logs (-edl): Enables debugging logs\n"
                 "--enable-trace-logs (-etl): Enables tracing logs. This will also enable debug logs, too\n"
                 "--dont-parse <argument> (--): Do not parse argument's beyond this point");
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

    Configuration* configuration = GetConfiguration(clientPath);

    switch (configuration->code) {
        case ERROR_CODE_BINARY:
            LOG_FATAL("Failed to load the binary: %s", configuration->errorMessage);
            return 1;

        case ERROR_CODE_ENTRY_NULL:
        case ERROR_CODE_NAME_NULL:
            LOG_FATAL("Failed to get important methods: %s", configuration->errorMessage);
            return 1;

        case ERROR_CODE_NULL:
            break;

        default:
            LOG_FATAL("Unknown error: %i", configuration->code);
            return 1;
    }

    LOG_INFO("Ready, starting '%s'", configuration->clientName);

    int returnValue = configuration->Start(configuration, argc, argv);

    free(configuration);
    LOG_INFO("Goodbye");

    return returnValue;
}