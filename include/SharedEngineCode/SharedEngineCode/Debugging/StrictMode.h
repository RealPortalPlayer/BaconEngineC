// Purpose: Does some strict checking.
// Created on: 7/29/22 @ 3:42 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdio.h>
#include <stdlib.h>

#include "SharedEngineCode/ArgumentHandler.h"
#include "SharedEngineCode/Logger.h"
#include "SharedEngineCode/BuiltInArguments.h"
#include "SharedEngineCode/Internal/Boolean.h"
#include "SharedEngineCode/MessageBox.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
SEC_Boolean SEC_StrictMode_CheckLogsEnabled(void);
SEC_Boolean SEC_StrictMode_IsEnabled(void);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()

#define SEC_STRICTMODE_CHECK(check, returnValue, ...) \
do {                                                  \
        if (SEC_StrictMode_CheckLogsEnabled())        \
            SEC_LOGGER_TRACE("Strict Mode checking: %s\n", #check); \
        if (!(check)) {                               \
            SEC_Logger_LogLevels level = SEC_LOGGER_LOG_LEVEL_FATAL; \
            if (!SEC_StrictMode_IsEnabled())          \
                level = SEC_LOGGER_LOG_LEVEL_ERROR;   \
            SEC_Logger_LogImplementation(SEC_BOOLEAN_TRUE, level, "Strict Check Failed\nCode: %s\nMessage: ", #check); \
            SEC_Logger_LogImplementation(SEC_BOOLEAN_FALSE, level, __VA_ARGS__);  \
            if (SEC_StrictMode_IsEnabled()) {         \
                SEC_MessageBox_Display(SEC_MESSAGEBOX_ICON_ERROR, SEC_MESSAGEBOX_BUTTON_OK, "BaconEngine - Strict Mode", "A strict check was triggered. More information can be found in the console. You can disable strict mode by using the \"--no-strict\", or the \"-ns\" argument.\n\nThe engine will now close."); \
                abort();                              \
                while (SEC_BOOLEAN_TRUE) continue;    \
            } else                                    \
                return returnValue;                   \
        }                                             \
} while (SEC_BOOLEAN_FALSE)

#define SEC_STRICTMODE_CHECK_NO_RETURN_VALUE(check, ...) SEC_STRICTMODE_CHECK(check, , __VA_ARGS__)