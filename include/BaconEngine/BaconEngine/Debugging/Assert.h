// Purpose: Make assertions easier.
// Created on: 3/30/22 @ 11:45 PM

#include <SharedEngineCode/Logger.h>
#include <stdlib.h>

#define ASSERT(check, ...) do { \
    LOG_TRACE("Assert checking: %s", #check); \
    if (!(check)) {             \
        LOG_FATAL("Assertion Failed!\nCode: %s\nMessage: %s", #check, __VA_ARGS__); \
        abort();                \
    }                           \
} while (0)