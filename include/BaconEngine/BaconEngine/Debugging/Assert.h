// Purpose: Make assertions easier.
// Created on: 3/30/22 @ 11:45 PM

#include <stdlib.h>
#include <stdio.h>
#include <SharedEngineCode/Logger.h>

SEC_CPP_SUPPORT_GUARD_START()
int BE_Assert_CheckLogsEnabled(void);
SEC_CPP_SUPPORT_GUARD_END()

#define BE_ASSERT(check, ...) \
do {                          \
    if (BE_Assert_CheckLogsEnabled()) \
        SEC_LOGGER_TRACE("Assert checking: %s\n", #check); \
    if (!(check)) {           \
        SEC_Logger_LogImplementation(1, SEC_LOGGER_LOG_LEVEL_FATAL, "Assertion Failed\nCode: %s\nMessage: ", #check); \
        SEC_Logger_LogImplementation(0, SEC_LOGGER_LOG_LEVEL_FATAL, __VA_ARGS__); \
        abort();              \
    }                         \
} while (0)

#define BE_ASSERT_ALWAYS(message) BE_ASSERT(0, message)
#define BE_ASSERT_NOT_IMPLEMENTED() BE_ASSERT_ALWAYS("This function is currently not implemented")
