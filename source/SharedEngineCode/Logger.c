#include <stdio.h>

#include "SharedEngineCode/Logger.h"
#include "SharedEngineCode/ANSICodes.h"
#include "SharedEngineCode/Internal/CppHeader.h"

CPP_GUARD_START()
    SEC_LogLevels currentLogLevel = SEC_LOG_LEVEL_INFO;

    SEC_LogLevels SEC_GetLogLevel(void) {
        return currentLogLevel;
    }

    void SEC_SetLogLevel(SEC_LogLevels logLevel) {
        currentLogLevel = logLevel;
    }

    void SEC_LogImplementation(int includeHeader, SEC_LogLevels logLevel, const char* message, ...) {
        if (currentLogLevel == SEC_LOG_LEVEL_NULL || logLevel < currentLogLevel)
            return;

        va_list arguments;

        if (includeHeader) {
            switch (logLevel) {
                case SEC_LOG_LEVEL_TRACE:
                    printf(SEC_ANSI_BRIGHT_FOREGROUND_BLUE "[TRC] " SEC_ANSI_RESET);
                    break;

                case SEC_LOG_LEVEL_DEBUG:
                    printf(SEC_ANSI_BRIGHT_FOREGROUND_MAGENTA "[DBG] " SEC_ANSI_RESET);
                    break;

                case SEC_LOG_LEVEL_INFO:
                    printf(SEC_ANSI_BRIGHT_FOREGROUND_CYAN "[INF] " SEC_ANSI_RESET);
                    break;

                case SEC_LOG_LEVEL_WARN:
                    printf(SEC_ANSI_BRIGHT_FOREGROUND_YELLOW "[WRN] " SEC_ANSI_RESET);
                    break;

                case SEC_LOG_LEVEL_ERROR:
                    printf(SEC_ANSI_BRIGHT_FOREGROUND_RED "[ERR] " SEC_ANSI_RESET);
                    break;

                case SEC_LOG_LEVEL_FATAL:
                    printf(SEC_ANSI_BOLD SEC_ANSI_FOREGROUND_RED "[FTL] " SEC_ANSI_RESET);
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
    }
CPP_GUARD_END()
