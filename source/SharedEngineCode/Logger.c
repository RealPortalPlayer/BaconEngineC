#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "SharedEngineCode/Logger.h"
#include "SharedEngineCode/ANSI.h"
#include "SharedEngineCode/Internal/CppSupport.h"
#include "SharedEngineCode/ArgumentHandler.h"
#include "SharedEngineCode/BuiltInArguments.h"
#include "SharedEngineCode/StringExtension.h"

SEC_CPP_SUPPORT_GUARD_START()
SEC_Logger_LogLevels currentLogLevel = SEC_LOGGER_LOG_LEVEL_INFO;

SEC_Logger_LogLevels SEC_Logger_GetLogLevel(void) {
    return currentLogLevel;
}

void SEC_Logger_SetLogLevel(SEC_Logger_LogLevels logLevel) {
    static int dontChangeLogLevels = -1;

    if (dontChangeLogLevels == -1)
        dontChangeLogLevels = SEC_ArgumentHandler_GetIndexWithShort(SEC_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS, SEC_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS_SHORT, 0, NULL, NULL);

    if (dontChangeLogLevels)
        return;

    currentLogLevel = logLevel;
}

// FIXME: This function is the opposite of thread safe. We need to fix that before making this engine multi-threaded.
void SEC_Logger_LogImplementation(int includeHeader, SEC_Logger_LogLevels logLevel, const char* message, ...) {
    {
        static int initialized = 0;

        if (!initialized) {
            char* specifiedLogLevel;

            SEC_ArgumentHandler_GetValueWithShort(SEC_BUILTINARGUMENTS_LOG_LEVEL, SEC_BUILTINARGUMENTS_LOG_LEVEL_SHORT, 0, &specifiedLogLevel, &specifiedLogLevel);

            if (specifiedLogLevel != NULL) {
                if (SEC_StringExtension_CompareCaseless(specifiedLogLevel, "null") == 0) // TODO: Tell the user if they specify a invalid log level.
                    currentLogLevel = SEC_LOGGER_LOG_LEVEL_NULL;
                else if (SEC_StringExtension_CompareCaseless(specifiedLogLevel, "trace") == 0 || SEC_StringExtension_CompareCaseless(specifiedLogLevel, "trc") == 0)
                    currentLogLevel = SEC_LOGGER_LOG_LEVEL_TRACE;
                else if (SEC_StringExtension_CompareCaseless(specifiedLogLevel, "debug") == 0 || SEC_StringExtension_CompareCaseless(specifiedLogLevel, "dbg") == 0)
                    currentLogLevel = SEC_LOGGER_LOG_LEVEL_DEBUG;
                else if (SEC_StringExtension_CompareCaseless(specifiedLogLevel, "warn") == 0 || SEC_StringExtension_CompareCaseless(specifiedLogLevel, "wrn") == 0)
                    currentLogLevel = SEC_LOGGER_LOG_LEVEL_WARN;
                else if (SEC_StringExtension_CompareCaseless(specifiedLogLevel, "error") == 0 || SEC_StringExtension_CompareCaseless(specifiedLogLevel, "err") == 0)
                    currentLogLevel = SEC_LOGGER_LOG_LEVEL_ERROR;
                else if (SEC_StringExtension_CompareCaseless(specifiedLogLevel, "fatal") == 0 || SEC_StringExtension_CompareCaseless(specifiedLogLevel, "ftl") == 0)
                    currentLogLevel = SEC_LOGGER_LOG_LEVEL_FATAL;
            }

            initialized = 1;
        }
    }

    if (currentLogLevel == SEC_LOGGER_LOG_LEVEL_NULL || logLevel > currentLogLevel)
        return;

    static int antiRecursiveLog = 0;

    if (antiRecursiveLog) {
        printf("Recursive log detected, this is 100%% a bug with the engine\n"
               "includeHeader: %i\n"
               "logLevel: %i\n"
               "message: %s\n", includeHeader, logLevel, message);
        abort();
    }

    antiRecursiveLog = 1;

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

    va_list arguments;

    va_start(arguments, message);
    printf("%s", SEC_ANSI_ConvertCodeToString(SEC_ANSI_CODE_RESET));
    vprintf(message, arguments);
    printf("\n");
    va_end(arguments);

    antiRecursiveLog = 0;
}
SEC_CPP_SUPPORT_GUARD_END()
