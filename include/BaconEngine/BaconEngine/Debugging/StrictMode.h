// Purpose: Does some strict checking.
// Created on: 7/29/22 @ 3:42 PM

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <stdio.h>
#include <stdlib.h>
#include <SharedEngineCode/BuiltInArguments.h>

#include "BaconEngine/ClientInformation.h"

#define BE_STRICTMODE_CHECK(check, returnValue, ...) \
do {                                                 \
        SEC_LOGGER_TRACE("Strict Mode checking: %s", #check); \
        if (!(check)) {                              \
            SEC_Logger_LogLevels level = SEC_LOGGER_LOG_LEVEL_FATAL; \
            if (!BE_ClientInformation_IsStrictModeEnabled()) \
                level = SEC_LOGGER_LOG_LEVEL_ERROR;  \
            SEC_Logger_LogImplementation(1, level, "Strict Check Failed\nCode: %s", #check); \
            if (SEC_Logger_GetLogLevel() <= level)   \
                printf("Message: ");                 \
            SEC_Logger_LogImplementation(0, level, __VA_ARGS__); \
            if (BE_ClientInformation_IsStrictModeEnabled()) \
                abort();                             \
            else                                     \
                return returnValue;                  \
        }                                            \
} while (0)
#define BE_STRICTMODE_CHECK_NO_RETURN_VALUE(check, ...) BE_STRICTMODE_CHECK(check, , __VA_ARGS__)
