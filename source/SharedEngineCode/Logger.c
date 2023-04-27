// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "SharedEngineCode/Logger.h"
#include "SharedEngineCode/ANSI.h"
#include "SharedEngineCode/Internal/CppSupport.h"
#include "SharedEngineCode/ArgumentHandler.h"
#include "SharedEngineCode/BuiltInArguments.h"
#include "SharedEngineCode/StringExtension.h"
#include "SharedEngineCode/Threads.h"

#if SEC_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <unistd.h>
#elif SEC_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#   include <io.h>
#   define fileno _fileno
#   define write(file, message, size) _write(file, message, (unsigned) size) // HACK: This is a stupid idea, but it's the only way to make MSVC shut up.
#endif

SEC_CPP_SUPPORT_GUARD_START()
SEC_Logger_LogLevels secLoggerCurrentLogLevel = SEC_LOGGER_LOG_LEVEL_INFO;
SEC_Thread_Lock secLoggerLock; // FIXME: This causes a memory leak

SEC_Logger_LogLevels SEC_Logger_GetLogLevel(void) {
    return secLoggerCurrentLogLevel;
}

void SEC_Logger_LogImplementation(int includeHeader, SEC_Logger_LogLevels logLevel, const char* message, ...) {
    static int alwaysUseStdout = -1;

    if (alwaysUseStdout == -1)
        alwaysUseStdout = SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_ALWAYS_USE_STDOUT,
                                                                      SEC_BUILTINARGUMENTS_ALWAYS_USE_STDOUT_SHORT, 0);

    {
        static SEC_Boolean initialized = SEC_FALSE;
        static SEC_Boolean initializing = SEC_FALSE;

        if (!initialized) {
            if (initializing) {
                fprintf(stderr, "Logger called self during initializing, this is 100%% a bug with the engine\n"
                                "includeHeader: %i\n"
                                "logLevel: %i\n"
                                "message: %s\n", includeHeader, logLevel, message);
                abort();
            }

            if (!SEC_Thread_CreateLock(&secLoggerLock)) {
                printf("Failed to initialize logger lock: %s (%i)\n", strerror(errno), errno);
                abort();
            }

            initializing = SEC_TRUE;

            {
                SEC_ArgumentHandler_ShortResults results;

                if (SEC_ArgumentHandler_GetInfoWithShort(SEC_BUILTINARGUMENTS_LOG_LEVEL,
                                                         SEC_BUILTINARGUMENTS_LOG_LEVEL_SHORT, 0, &results) != 0) {
                    if (SEC_StringExtension_CompareCaseless(*results.value, "null") == 0)
                        secLoggerCurrentLogLevel = SEC_LOGGER_LOG_LEVEL_NULL;
                    else if (SEC_StringExtension_CompareCaseless(*results.value, "trace") == 0 ||
                             SEC_StringExtension_CompareCaseless(*results.value, "trc") == 0)
                        secLoggerCurrentLogLevel = SEC_LOGGER_LOG_LEVEL_TRACE;
                    else if (SEC_StringExtension_CompareCaseless(*results.value, "debug") == 0 ||
                             SEC_StringExtension_CompareCaseless(*results.value, "dbg") == 0)
                        secLoggerCurrentLogLevel = SEC_LOGGER_LOG_LEVEL_DEBUG;
                    else if (SEC_StringExtension_CompareCaseless(*results.value, "warn") == 0 ||
                             SEC_StringExtension_CompareCaseless(*results.value, "wrn") == 0)
                        secLoggerCurrentLogLevel = SEC_LOGGER_LOG_LEVEL_WARN;
                    else if (SEC_StringExtension_CompareCaseless(*results.value, "error") == 0 ||
                             SEC_StringExtension_CompareCaseless(*results.value, "err") == 0)
                        secLoggerCurrentLogLevel = SEC_LOGGER_LOG_LEVEL_ERROR;
                    else if (SEC_StringExtension_CompareCaseless(*results.value, "fatal") == 0 ||
                             SEC_StringExtension_CompareCaseless(*results.value, "ftl") == 0)
                        secLoggerCurrentLogLevel = SEC_LOGGER_LOG_LEVEL_FATAL;
                    else if (SEC_StringExtension_CompareCaseless(*results.value, "info") != 0 &&
                             SEC_StringExtension_CompareCaseless(*results.value, "inf") != 0) {
                        SEC_Logger_LogHeader(stderr, SEC_LOGGER_LOG_LEVEL_ERROR);
                        printf("Invalid log type: %s\n", *results.value);
                    }
                }
            }

            initialized = SEC_TRUE;
            initializing = SEC_FALSE;
        }
    }

    SEC_Thread_UseLock(&secLoggerLock);

    if (!SEC_Logger_IsLevelEnabled(logLevel)) {
        SEC_Thread_Unlock(&secLoggerLock);
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
        SEC_Logger_LogHeader(output, logLevel);

    va_list arguments;

    va_start(arguments, message);
    vfprintf(output, message, arguments);
    va_end(arguments);

    antiRecursiveLog = SEC_FALSE;

    SEC_Thread_Unlock(&secLoggerLock);
}

void SEC_Logger_LogHeader(FILE* output, SEC_Logger_LogLevels logLevel) {
    if (!SEC_Logger_IsLevelEnabled(logLevel))
        return;

    {
        static int dontLogHeader = -1;

        if (dontLogHeader == -1)
            dontLogHeader = SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_DISABLE_LOG_HEADER, SEC_BUILTINARGUMENTS_DISABLE_LOG_HEADER_SHORT, 0);

        if (dontLogHeader)
            return;
    }

#define SEC_LOGGER_SAFE_PUTS(message) write(fileno(output), message, strlen(message))
    switch (logLevel) {
        case SEC_LOGGER_LOG_LEVEL_TRACE:
            if (SEC_ANSI_IsEnabled())
                SEC_LOGGER_SAFE_PUTS(SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_BRIGHT_FOREGROUND_BLUE));

            SEC_LOGGER_SAFE_PUTS("[TRC] ");
            break;

        case SEC_LOGGER_LOG_LEVEL_DEBUG:
            if (SEC_ANSI_IsEnabled())
                SEC_LOGGER_SAFE_PUTS(SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_BRIGHT_FOREGROUND_MAGENTA));

            SEC_LOGGER_SAFE_PUTS("[DBG] ");
            break;

        case SEC_LOGGER_LOG_LEVEL_INFO:
            if (SEC_ANSI_IsEnabled())
                SEC_LOGGER_SAFE_PUTS(SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_BRIGHT_FOREGROUND_CYAN));

            SEC_LOGGER_SAFE_PUTS("[INF] ");
            break;

        case SEC_LOGGER_LOG_LEVEL_WARN:
            if (SEC_ANSI_IsEnabled())
                SEC_LOGGER_SAFE_PUTS(SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_BRIGHT_FOREGROUND_YELLOW));

            SEC_LOGGER_SAFE_PUTS("[WRN] ");
            break;

        case SEC_LOGGER_LOG_LEVEL_ERROR:
            if (SEC_ANSI_IsEnabled())
                SEC_LOGGER_SAFE_PUTS(SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_BRIGHT_FOREGROUND_RED));

            SEC_LOGGER_SAFE_PUTS("[ERR] ");
            break;

        case SEC_LOGGER_LOG_LEVEL_FATAL:
            if (SEC_ANSI_IsEnabled()) {
                SEC_LOGGER_SAFE_PUTS(SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_BOLD));
                SEC_LOGGER_SAFE_PUTS(SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_FOREGROUND_RED));
            }

            SEC_LOGGER_SAFE_PUTS("[FTL] ");
            break;

        default:
            SEC_LOGGER_SAFE_PUTS("[UNK] ");
            break;
    }

    if (SEC_ANSI_IsEnabled())
        SEC_LOGGER_SAFE_PUTS(SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_RESET));
#undef SEC_LOGGER_SAFE_PUTS
}

SEC_Boolean SEC_Logger_IsLevelEnabled(SEC_Logger_LogLevels logLevel) {
    return secLoggerCurrentLogLevel != SEC_LOGGER_LOG_LEVEL_NULL && logLevel <= secLoggerCurrentLogLevel;
}

void SEC_Logger_SetLogLevel(SEC_Logger_LogLevels logLevel) {
    static int dontChangeLogLevels = -1;

    if (dontChangeLogLevels == -1)
        dontChangeLogLevels = SEC_ArgumentHandler_ContainsArgumentOrShort(SEC_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS, SEC_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS_SHORT, 0);

    if (dontChangeLogLevels)
        return;

    secLoggerCurrentLogLevel = logLevel;
}
SEC_CPP_SUPPORT_GUARD_END()
