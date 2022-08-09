#include <stdio.h>
#include <pthread.h>

#include "SharedEngineCode/Logger.h"
#include "SharedEngineCode/ANSICodes.h"
#include "SharedEngineCode/Internal/CppHeader.h"

CPP_GUARD_START()
    volatile LogLevels currentLogLevel = LOG_LEVEL_INFO;

    pthread_mutex_t loggingLock;

    void LogImplementation(int includeHeader, LogLevels logLevel, const char* message, ...) {
        if (currentLogLevel == LOG_LEVEL_NULL || logLevel < currentLogLevel)
            return;

        pthread_mutex_lock(&loggingLock);

        va_list arguments;

        if (includeHeader) {
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
        }

        va_start(arguments, message);
        vprintf(message, arguments);
        printf("\n");
        va_end(arguments);
        pthread_mutex_unlock(&loggingLock);
    }
CPP_GUARD_END()
