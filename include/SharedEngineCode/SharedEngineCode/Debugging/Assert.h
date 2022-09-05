// Purpose: Make assertions easier.
// Created on: 3/30/22 @ 11:45 PM

#include <stdlib.h>
#include <stdio.h>

#include "SharedEngineCode/Logger.h"
#include "SharedEngineCode/EngineMemory.h"

#define SEC_ASSERT(check, ...) \
do {                       \
    SEC_LOGGER_TRACE("Assert checking: %s", #check); \
    if (!(check)) {        \
        SEC_LOGGER_FATAL("Assertion Failed\nCode: %s", #check); \
        printf("Message: "); \
        SEC_Logger_LogImplementation(0, SEC_LOGGER_LOG_LEVEL_FATAL, __VA_ARGS__); \
        abort();           \
    }                      \
} while (0)

#define SEC_ASSERT_MALLOC(destination, size, forWhat) \
SEC_EngineMemory_AddAllocated(size);                  \
destination = malloc(size);                           \
SEC_ASSERT(destination != NULL, "Failed to allocate %lu bytes of data for %s", size, forWhat)

#define SEC_ASSERT_REALLOC(destination, oldSize, size, forWhat) \
SEC_EngineMemory_RemoveAllocated(oldSize);                      \
SEC_EngineMemory_AddAllocated(size);                            \
destination = realloc(destination, size);                       \
SEC_ASSERT(destination != NULL, "Failed to reallocate %lu bytes of data for %s", size, forWhat)
