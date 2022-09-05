#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "SharedEngineCode/Logger.h"
#include "SharedEngineCode/ANSI.h"
#include "SharedEngineCode/Internal/CppHeader.h"
#include "SharedEngineCode/ArgumentHandler.h"

SEC_CPP_GUARD_START()
    SEC_Logger_LogLevels currentLogLevel = SEC_LOGGER_LOG_LEVEL_INFO;
    const char* lastMessage;
    SEC_Logger_LogLevels lastLogLevel;
    int sentAmount = 1;
    int firstLog = 1;

    SEC_Logger_LogLevels SEC_Logger_GetLogLevel(void) {
        return currentLogLevel;
    }

    void SEC_Logger_SetLogLevel(SEC_Logger_LogLevels logLevel) {
        static int dontChangeLogLevels = -1;

        if (dontChangeLogLevels == -1)
            dontChangeLogLevels = SEC_ArgumentHandler_GetIndex("--dont-change-log-levels") != -1 ||
                    SEC_ArgumentHandler_GetIndex("-dcll") != -1;

        if (dontChangeLogLevels)
            return;

        currentLogLevel = logLevel;
    }

    // FIXME: This function is the opposite of thread safe. We need to fix that before making this engine multi-threaded.
    void SEC_Logger_LogImplementation(int includeHeader, SEC_Logger_LogLevels logLevel, const char* message, ...) {
        {
            static int initialized = 0;

            if (!initialized) {
                char* specifiedLogLevel = (char*) SEC_ArgumentHandler_GetValue("--log-level");

                if (specifiedLogLevel == NULL)
                    specifiedLogLevel = (char*) SEC_ArgumentHandler_GetValue("-ll");

                if (specifiedLogLevel != NULL) {
                    for (int i = 0; i < strlen(specifiedLogLevel); i++)
                        specifiedLogLevel[i] = (char) tolower(specifiedLogLevel[i]);

                    if (strcmp(specifiedLogLevel, "null") == 0)
                        currentLogLevel = SEC_LOGGER_LOG_LEVEL_NULL;
                    else if (strcmp(specifiedLogLevel, "trace") == 0 || strcmp(specifiedLogLevel, "trc") == 0)
                        currentLogLevel = SEC_LOGGER_LOG_LEVEL_TRACE;
                    else if (strcmp(specifiedLogLevel, "debug") == 0 || strcmp(specifiedLogLevel, "dbg") == 0)
                        currentLogLevel = SEC_LOGGER_LOG_LEVEL_DEBUG;
                    else if (strcmp(specifiedLogLevel, "warn") == 0 || strcmp(specifiedLogLevel, "wrn") == 0)
                        currentLogLevel = SEC_LOGGER_LOG_LEVEL_WARN;
                    else if (strcmp(specifiedLogLevel, "error") == 0 || strcmp(specifiedLogLevel, "err") == 0)
                        currentLogLevel = SEC_LOGGER_LOG_LEVEL_ERROR;
                    else if (strcmp(specifiedLogLevel, "fatal") == 0 || strcmp(specifiedLogLevel, "ftl") == 0)
                        currentLogLevel = SEC_LOGGER_LOG_LEVEL_FATAL;
                }

                initialized = 1;
            }
        }

        if (currentLogLevel == SEC_LOGGER_LOG_LEVEL_NULL || logLevel < currentLogLevel)
            return;

        static int antiRecursiveLog = 0;
        static int compressDupeLogs = -1;

        if (compressDupeLogs == -1)
            compressDupeLogs = SEC_ArgumentHandler_GetIndex("--dont-compress-dupe-logs") == -1 &&
                    SEC_ArgumentHandler_GetIndex("-dcdl") == -1;

        if (antiRecursiveLog) {
            printf("Recursive log detected, this is 100%% a bug with the engine\n"
                   "includeHeader: %i\n"
                   "logLevel: %i\n"
                   "message: %s\n", includeHeader, logLevel, message);
            abort();
        }

        antiRecursiveLog = 1;

        va_list arguments;

        va_start(arguments, message);

        int size = vsnprintf(NULL, 0, message, arguments) + 1;

        va_end(arguments);
        va_start(arguments, message);

        // NOTE: Do NOT replace this with SEC_ASSERT_MALLOC, or SEC_ASSERT. They both call this function!!!

        char* buffer = malloc(sizeof(char) * size);

        if (buffer == NULL) {
            printf("Internal malloc failed, failed to allocate %lu bytes of memory\n", sizeof(char) * size);
            abort();
        }

        vsnprintf(buffer, size, message, arguments);

        if (compressDupeLogs) {
            if (lastMessage == NULL || strcmp(lastMessage, buffer) != 0 || logLevel != lastLogLevel) {
                lastMessage = buffer;
                lastLogLevel = logLevel;
                sentAmount = 1;
            } else {
                sentAmount++;

                free(buffer);

                buffer = (char*) lastMessage;
            }
        }

        if (!firstLog)
            printf(sentAmount > 1 && compressDupeLogs ? "\r" : "\n");

        firstLog = 0;

        if (includeHeader)
            switch (logLevel) {
                case SEC_LOGGER_LOG_LEVEL_TRACE:
                    printf("%s[TRC] ", SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_BRIGHT_FOREGROUND_BLUE));
                    break;

                case SEC_LOGGER_LOG_LEVEL_DEBUG:
                    printf("%s[DBG] ", SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_BRIGHT_FOREGROUND_MAGENTA));
                    break;

                case SEC_LOGGER_LOG_LEVEL_INFO:
                    printf("%s[INF] ", SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_BRIGHT_FOREGROUND_CYAN));
                    break;

                case SEC_LOGGER_LOG_LEVEL_WARN:
                    printf("%s[WRN] ", SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_BRIGHT_FOREGROUND_YELLOW));
                    break;

                case SEC_LOGGER_LOG_LEVEL_ERROR:
                    printf("%s[ERR] ", SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_BRIGHT_FOREGROUND_RED));
                    break;

                case SEC_LOGGER_LOG_LEVEL_FATAL:
                    printf("%s%s[FTL] ", SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_BOLD),
                           SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_FOREGROUND_RED));
                    break;

                default:
                    printf("[UNK] ");
                    break;
            }

        printf("%s", SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_RESET));
        printf("%s", buffer);
        va_end(arguments);

        if (sentAmount > 1 && compressDupeLogs) {
            printf(" %s(x%i)%s", SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_BRIGHT_FOREGROUND_BLACK), sentAmount,
                   SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_RESET));
            fflush(stdout);
        }

        antiRecursiveLog = 0;
    }

    int SEC_Logger_AlreadySentFistLog(void) {
        return !firstLog;
    }
SEC_CPP_GUARD_END()
