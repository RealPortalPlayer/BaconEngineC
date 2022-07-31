// Purpose: Make assertions easier.
// Created on: 3/30/22 @ 11:45 PM

#include <SharedEngineCode/Logger.h>
#include <stdlib.h>
#include <stdio.h>

#define ASSERT(check, ...) \
do {                       \
    LOG_TRACE("Assert checking: %s", #check); \
    if (!(check)) {        \
        LOG_FATAL("Assertion Failed\nCode: %s", #check); \
        printf("Message: "); \
        LogImplementation(0, LOG_LEVEL_FATAL, __VA_ARGS__); \
        abort();           \
    }                      \
} while (0)