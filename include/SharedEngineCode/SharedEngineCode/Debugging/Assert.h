// Purpose: Make assertions easier.
// Created on: 3/30/22 @ 11:45 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <stdlib.h>
#include <stdio.h>

#include "SharedEngineCode/Logger.h"
#include "SharedEngineCode/Internal/Boolean.h"
#include "SharedEngineCode/MessageBox.h"

SEC_CPLUSPLUS_SUPPORT_GUARD_START()
SEC_Boolean SEC_Assert_CheckLogsEnabled(void);
SEC_CPLUSPLUS_SUPPORT_GUARD_END()

#define SEC_ASSERT(check, ...) \
do {                          \
    if (SEC_Assert_CheckLogsEnabled()) \
        SEC_LOGGER_TRACE("Assert checking: %s\n", #check); \
    if (!(check)) {           \
        SEC_Logger_LogImplementation(SEC_BOOLEAN_TRUE, SEC_LOGGER_LOG_LEVEL_FATAL, "Assertion Failed\nCode: %s\nMessage: ", #check); \
        SEC_Logger_LogImplementation(SEC_BOOLEAN_FALSE, SEC_LOGGER_LOG_LEVEL_FATAL, __VA_ARGS__); \
        SEC_MessageBox_Display(SEC_MESSAGEBOX_ICON_ERROR, SEC_MESSAGEBOX_BUTTON_OK, "BaconEngine - Assert", "An assert was triggered. More information can be found in the console.\n\nThe engine will now close."); \
        abort();              \
        while (SEC_BOOLEAN_TRUE) continue; \
    }                         \
} while (SEC_BOOLEAN_FALSE)

#define SEC_ASSERT_ALWAYS(message) SEC_ASSERT(SEC_BOOLEAN_FALSE, message)
#define SEC_ASSERT_NOT_IMPLEMENTED() SEC_ASSERT_ALWAYS("This function is currently not implemented\n")
