// Purpose: Does some strict checking.
// Created on: 7/29/22 @ 3:42 PM

// Copyright (c) 2022, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <SharedEngineCode/ArgumentHandler.h>
#include <SharedEngineCode/Logger.h>
#include <stdio.h>
#include <stdlib.h>
#include <SharedEngineCode/BuiltInArguments.h>

#include "BaconEngine/ClientInformation.h"

SEC_CPP_SUPPORT_GUARD_START()
int BE_StrictMode_CheckLogsEnabled(void);
SEC_CPP_SUPPORT_GUARD_END()

#define BE_STRICTMODE_CHECK(check, returnValue, ...) \
do {                                                 \
        if (BE_StrictMode_CheckLogsEnabled())        \
            SEC_LOGGER_TRACE("Strict Mode checking: %s\n", #check); \
        if (!(check)) {                              \
            SEC_Logger_LogLevels level = SEC_LOGGER_LOG_LEVEL_FATAL; \
            if (!BE_ClientInformation_IsStrictModeEnabled()) \
                level = SEC_LOGGER_LOG_LEVEL_ERROR;  \
            SEC_Logger_LogImplementation(1, level, "Strict Check Failed\nCode: %s\nMessage: ", #check); \
            SEC_Logger_LogImplementation(0, level, __VA_ARGS__);  \
            if (BE_ClientInformation_IsStrictModeEnabled()) \
                abort();                             \
            else                                     \
                return returnValue;                  \
        }                                            \
} while (0)

#define BE_STRICTMODE_CHECK_NO_RETURN_VALUE(check, ...) BE_STRICTMODE_CHECK(check, , __VA_ARGS__)
