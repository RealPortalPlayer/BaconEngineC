#include <stdio.h>
#include <string.h>
#include <Windows.h>

#include "SharedEngineCode/Logger.h"
#include "SharedEngineCode/ANSICodes.h"
#include "SharedEngineCode/Internal/CppHeader.h"
#include "SharedEngineCode/ArgumentHandler.h"

CPP_GUARD_START()
    SEC_LogLevels currentLogLevel = SEC_LOG_LEVEL_INFO;
    const char* lastMessage;
    va_list lastVaList;
    int sentAmount = 1;
    int firstLog = 1;

    SEC_LogLevels SEC_GetLogLevel(void) {
        return currentLogLevel;
    }

    void SEC_SetLogLevel(SEC_LogLevels logLevel) {
        currentLogLevel = logLevel;
    }

    void SEC_LogImplementation(int includeHeader, SEC_LogLevels logLevel, const char* message, ...) {
        if (currentLogLevel == SEC_LOG_LEVEL_NULL || logLevel < currentLogLevel)
            return;

        static int useAnsi = -1;

        if (useAnsi == -1)
            useAnsi = SEC_GetArgumentIndex("--disable-ansi-coloring") == -1 && SEC_GetArgumentIndex("-dac") == -1;

        va_list arguments;

        va_start(arguments, message);

        char* buffer;
        int size = vsnprintf(NULL, 0, message, arguments) + 1;

        // NOTE: Do NOT replace this with SEC_ASSERT_MALLOC, or SEC_ASSERT. They both call this function!!!

        buffer = malloc(sizeof(char) * size);

        if (buffer == NULL) {
            printf("Internal malloc failed, failed to allocate %llu bytes of memory\n", sizeof(char) * size);
            abort();
        }

        vsnprintf(buffer, size, message, arguments);

        if (lastMessage == NULL || strcmp(lastMessage, buffer) != 0) {
            lastMessage = buffer;
            sentAmount = 1;
        } else {
            sentAmount++;

            free(buffer);

            buffer = (char*) lastMessage;
        }

        printf("\r");


        firstLog = 0;

        if (includeHeader) {
            switch (logLevel) {
                case SEC_LOG_LEVEL_TRACE:
                    printf("%s[TRC] ", useAnsi ? SEC_ANSI_BRIGHT_FOREGROUND_BLUE : "");
                    break;

                case SEC_LOG_LEVEL_DEBUG:
                    printf("%s[DBG] ", useAnsi ? SEC_ANSI_BRIGHT_FOREGROUND_MAGENTA : "");
                    break;

                case SEC_LOG_LEVEL_INFO:
                    printf("%s[INF] ", useAnsi ? SEC_ANSI_BRIGHT_FOREGROUND_CYAN : "");
                    break;

                case SEC_LOG_LEVEL_WARN:
                    printf("%s[WRN] ", useAnsi ? SEC_ANSI_BRIGHT_FOREGROUND_YELLOW : "");
                    break;

                case SEC_LOG_LEVEL_ERROR:
                    printf("%s[ERR] ", useAnsi ? SEC_ANSI_BRIGHT_FOREGROUND_RED : "");
                    break;

                case SEC_LOG_LEVEL_FATAL:
                    printf("%s[FTL] ", useAnsi ? SEC_ANSI_BOLD SEC_ANSI_FOREGROUND_RED : "");
                    break;

                default:
                    printf("[UNK] ");
                    break;
            }
        }

        if (useAnsi)
            printf(SEC_ANSI_RESET);

        printf("%s", buffer);
        va_end(arguments);

        if (sentAmount > 1) {
            printf(" (x%i)", sentAmount);
            fflush(stdout);
        }
    }
CPP_GUARD_END()
