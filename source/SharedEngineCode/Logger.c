#include <stdio.h>

#include "SharedEngineCode/Logger.h"
#include "SharedEngineCode/ANSICodes.h"

#ifdef __cplusplus
extern "C" {
#endif

volatile LogLevels currentLogLevel = LOG_LEVEL_INFO;

void LogImplementation(LogLevels logLevel, const char* message, ...) {
    if (currentLogLevel == LOG_LEVEL_NULL || logLevel < currentLogLevel)
        return;

    va_list arguments;

    switch (logLevel) {
        case LOG_LEVEL_TRACE:
            printf(ANSI_BRIGHT_FOREGROUND_BLUE "[TRC] " ANSI_RESET);
            break;

        case LOG_LEVEL_DEBUG:
            printf(ANSI_BRIGHT_FOREGROUND_MAGENTA "[DBG] " ANSI_RESET);
            break;

        case LOG_LEVEL_INFO:
            printf(ANSI_BRIGHT_FOREGROUND_CYAN "[INF] " ANSI_RESET);
            break;

        case LOG_LEVEL_WARN:
            printf(ANSI_BRIGHT_FOREGROUND_YELLOW "[WRN] " ANSI_RESET);
            break;

        case LOG_LEVEL_ERROR:
            printf(ANSI_BRIGHT_FOREGROUND_RED "[ERR] " ANSI_RESET);
            break;

        case LOG_LEVEL_FATAL:
            printf(ANSI_BOLD ANSI_FOREGROUND_RED "[FTL] " ANSI_RESET);
            break;

        default:
            printf("[UNK] ");
            break;
    }

    va_start(arguments, message);
    vprintf(message, arguments);
    printf("\n");
    va_end(arguments);
}

#ifdef __cplusplus
}
#endif