// Purpose: Does some strict checking.
// Created on: 7/29/22 @ 3:42 PM

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <stdio.h>
#include <stdlib.h>

#include "BaconEngine/ClientInformation.h"

#define STRICT_CHECK(check, returnValue, ...) \
do {                                          \
        LOG_TRACE("Strict Mode checking: %s", #check); \
        if (!(check)) {                       \
            LogLevels level = LOG_LEVEL_FATAL; \
            int strictMode = IsStrictModeEnabled(); \
            if (!strictMode)                  \
                level = LOG_LEVEL_ERROR;      \
            LogImplementation(1, level, "Strict Check Failed\nCode: %s", #check); \
            printf("Message: ");              \
            LogImplementation(0, level, __VA_ARGS__); \
            if (strictMode)                   \
                abort();                      \
            else                              \
                return returnValue;           \
        }                                     \
} while (0)
#define STRICT_CHECK_NO_RETURN_VALUE(check, ...) STRICT_CHECK(check, , __VA_ARGS__)