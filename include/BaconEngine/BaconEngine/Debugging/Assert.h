// Purpose: Make assertions easier.
// Created on: 3/30/22 @ 11:45 PM

#include <SharedEngineCode/Logger.h>
#include <stdlib.h>
#include <stdio.h>

#define BE_ASSERT(check, ...) \
do {                       \
    SEC_LOG_TRACE("Assert checking: %s", #check); \
    if (!(check)) {        \
        SEC_LOG_FATAL("Assertion Failed\nCode: %s", #check); \
        printf("Message: "); \
        SEC_LogImplementation(0, SEC_LOG_LEVEL_FATAL, __VA_ARGS__); \
        abort();           \
    }                      \
} while (0)

#define BE_ASSERT_MALLOC(destination, size, forWhat) \
destination = malloc(size);                       \
BE_ASSERT(destination != NULL, "Failed to allocate %lu bytes of data for %s", size, forWhat)

#define BE_ASSERT_REALLOC(destination, size, forWhat) \
destination = realloc(destination, size);          \
BE_ASSERT(destination != NULL, "Failed to reallocate %lu bytes of data for %s", size, forWhat)
