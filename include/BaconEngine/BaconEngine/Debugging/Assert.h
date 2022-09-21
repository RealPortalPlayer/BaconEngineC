// Purpose: Make assertions easier.
// Created on: 3/30/22 @ 11:45 PM

#include <stdlib.h>
#include <stdio.h>
#include <SharedEngineCode/Logger.h>

#define BE_ASSERT(check, ...) \
do {                          \
    SEC_LOGGER_TRACE("Assert checking: %s", #check); \
    if (!(check)) {           \
        SEC_LOGGER_FATAL("Assertion Failed\nCode: %s", #check); \
        printf("Message: ");\
        SEC_Logger_LogImplementation(0, SEC_LOGGER_LOG_LEVEL_FATAL, __VA_ARGS__); \
        abort();              \
    }                         \
} while (0)
