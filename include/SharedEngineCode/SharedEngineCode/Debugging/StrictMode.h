// Purpose: Does some strict checking.
// Created on: 7/29/22 @ 3:42 PM

#include "SharedEngineCode/ArgumentHandler.h"
#include "SharedEngineCode/Logger.h"
#include <stdio.h>
#include <stdlib.h>

#define SEC_STRICTMODE_CHECK(check, returnValue, ...) \
do {                                                  \
        SEC_LOGGER_TRACE("Strict Mode checking: %s", #check); \
        if (!(check)) {                               \
            SEC_Logger_LogLevels level = SEC_LOGGER_LOG_LEVEL_FATAL; \
            static int SEC_strictModeEnabledAddedPaddingToPreventNameCollisionsPleaseDontCollide = -1; /* HACK: This is dumb */ \
            if (SEC_strictModeEnabledAddedPaddingToPreventNameCollisionsPleaseDontCollide == -1) \
                SEC_strictModeEnabledAddedPaddingToPreventNameCollisionsPleaseDontCollide = SEC_ArgumentHandler_GetIndex("--no-strict") == -1 && SEC_ArgumentHandler_GetIndex("-ns") == -1; \
            if (!SEC_strictModeEnabledAddedPaddingToPreventNameCollisionsPleaseDontCollide) \
                level = SEC_LOGGER_LOG_LEVEL_ERROR;   \
            SEC_Logger_LogImplementation(1, level, "Strict Check Failed\nCode: %s", #check); \
            printf("Message: ");                      \
            SEC_Logger_LogImplementation(0, level, __VA_ARGS__); \
            if (SEC_strictModeEnabledAddedPaddingToPreventNameCollisionsPleaseDontCollide) \
                abort();                              \
            else                                      \
                return returnValue;                   \
        }                                             \
} while (0)
#define SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(check, ...) SEC_STRICTMODE_CHECK(check, , __VA_ARGS__)
