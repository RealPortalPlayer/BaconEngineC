// Purpose: Does some strict checking.
// Created on: 7/29/22 @ 3:42 PM

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <stdio.h>
#include <stdlib.h>

#include "BaconEngine/ClientInformation.h"

#define BE_STRICT_CHECK(check, returnValue, ...) \
do {                                             \
        SEC_LOG_TRACE("Strict Mode checking: %s", #check); \
        if (!(check)) {                          \
            SEC_LogLevels level = SEC_LOG_LEVEL_FATAL; \
            int strictMode = BE_IsStrictModeEnabled(); \
            if (!strictMode)                     \
                level = SEC_LOG_LEVEL_ERROR;      \
            SEC_LogImplementation(1, level, "Strict Check Failed\nCode: %s", #check); \
            printf("Message: ");                 \
            SEC_LogImplementation(0, level, __VA_ARGS__); \
            if (strictMode)                      \
                abort();                         \
            else                                 \
                return returnValue;              \
        }                                        \
} while (0)
#define BE_STRICT_CHECK_NO_RETURN_VALUE(check, ...) BE_STRICT_CHECK(check, , __VA_ARGS__)
