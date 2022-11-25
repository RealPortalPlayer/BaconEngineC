// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "SharedEngineCode/Logger.h"
#include "SharedEngineCode/ANSI.h"
#include "SharedEngineCode/Internal/CppSupport.h"
#include "SharedEngineCode/ArgumentHandler.h"
#include "SharedEngineCode/BuiltInArguments.h"
#include "SharedEngineCode/StringExtension.h"

SEC_CPP_SUPPORT_GUARD_START()
SEC_Logger_LogLevels beLoggerCurrentLogLevel = SEC_LOGGER_LOG_LEVEL_INFO;
pthread_mutex_t beLoggerLock;

SEC_Logger_LogLevels SEC_Logger_GetLogLevel(void) {
    return beLoggerCurrentLogLevel;
}

void SEC_Logger_SetLogLevel(SEC_Logger_LogLevels logLevel) {
    static int dontChangeLogLevels = -1;

    if (dontChangeLogLevels == -1)
        dontChangeLogLevels = SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS, SEC_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS_SHORT, 0);

    if (dontChangeLogLevels)
        return;

    beLoggerCurrentLogLevel = logLevel;
}

void SEC_Logger_LogImplementation(int includeHeader, SEC_Logger_LogLevels logLevel, const char* message, ...) {
     static int alwaysUseStdout = -1;

    if (alwaysUseStdout == -1)
        alwaysUseStdout = SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_ALWAYS_USE_STDOUT, SEC_BUILTINARGUMENTS_ALWAYS_USE_STDOUT_SHORT, 0);

    {
        static SEC_Boolean initialized = SEC_FALSE;
        static SEC_Boolean initializing = SEC_FALSE;

        if (!initialized) {
            pthread_mutex_init(&beLoggerLock, NULL);

            SEC_ArgumentHandler_ShortResults results;

            if (initializing) {
                fprintf(stderr, "Logger called self during initializing, this is 100%% a bug with the engine\n"
                                "includeHeader: %i\n"
                                "logLevel: %i\n"
                                "message: %s\n", includeHeader, logLevel, message);
                abort();
            }

            initializing = SEC_TRUE;

            if (SEC_ArgumentHandler_GetInfoWithShort(SEC_BUILTINARGUMENTS_LOG_LEVEL, SEC_BUILTINARGUMENTS_LOG_LEVEL_SHORT, 0, &results) != 0) {
                if (SEC_StringExtension_CompareCaseless(*results.value, "null") == 0) // TODO: Tell the user if they specify a invalid log level.
                    beLoggerCurrentLogLevel = SEC_LOGGER_LOG_LEVEL_NULL;
                else if (SEC_StringExtension_CompareCaseless(*results.value, "trace") == 0 || SEC_StringExtension_CompareCaseless(*results.value, "trc") == 0)
                    beLoggerCurrentLogLevel = SEC_LOGGER_LOG_LEVEL_TRACE;
                else if (SEC_StringExtension_CompareCaseless(*results.value, "debug") == 0 || SEC_StringExtension_CompareCaseless(*results.value, "dbg") == 0)
                    beLoggerCurrentLogLevel = SEC_LOGGER_LOG_LEVEL_DEBUG;
                else if (SEC_StringExtension_CompareCaseless(*results.value, "warn") == 0 || SEC_StringExtension_CompareCaseless(*results.value, "wrn") == 0)
                    beLoggerCurrentLogLevel = SEC_LOGGER_LOG_LEVEL_WARN;
                else if (SEC_StringExtension_CompareCaseless(*results.value, "error") == 0 || SEC_StringExtension_CompareCaseless(*results.value, "err") == 0)
                    beLoggerCurrentLogLevel = SEC_LOGGER_LOG_LEVEL_ERROR;
                else if (SEC_StringExtension_CompareCaseless(*results.value, "fatal") == 0 || SEC_StringExtension_CompareCaseless(*results.value, "ftl") == 0)
                    beLoggerCurrentLogLevel = SEC_LOGGER_LOG_LEVEL_FATAL;
            }

            initialized = SEC_TRUE;
            initializing = SEC_FALSE;
        }
    }

    pthread_mutex_lock(&beLoggerLock);

    if (beLoggerCurrentLogLevel == SEC_LOGGER_LOG_LEVEL_NULL || logLevel > beLoggerCurrentLogLevel) {
        pthread_mutex_unlock(&beLoggerLock);
        return;
    }

    static SEC_Boolean antiRecursiveLog = SEC_FALSE;
    FILE* output = stdout;

    if (!alwaysUseStdout && (logLevel == SEC_LOGGER_LOG_LEVEL_ERROR || logLevel == SEC_LOGGER_LOG_LEVEL_FATAL))
        output = stderr;

    if (antiRecursiveLog) {
        fprintf(output, "Logger called self, this is 100%% a bug with the engine\n"
               "includeHeader: %i\n"
               "logLevel: %i\n"
               "message: %s\n", includeHeader, logLevel, message);
        abort();
    }

    antiRecursiveLog = SEC_TRUE;

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
                fprintf(output, "%s[ERR] ", SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_BRIGHT_FOREGROUND_RED));
                break;

            case SEC_LOGGER_LOG_LEVEL_FATAL:
                fprintf(output, "%s%s[FTL] ", SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_BOLD),
                        SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_FOREGROUND_RED));
                break;

            default:
                printf("[UNK] ");
                break;
        }

    va_list arguments;

    va_start(arguments, message);
    fprintf(output, "%s", SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_RESET));
    vfprintf(output, message, arguments);
    va_end(arguments);

    antiRecursiveLog = SEC_FALSE;

    pthread_mutex_unlock(&beLoggerLock);
}
SEC_CPP_SUPPORT_GUARD_END()
