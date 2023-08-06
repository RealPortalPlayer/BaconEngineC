// Purpose: Wrapper around interface specific functions
// Created on: 4/22/2023 @ 3:10 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#ifdef BE_CLIENT_BINARY
#   include <Interface/Engine.h>
#   include <SharedEngineCode/Internal/OperatingSystem.h>
#   include <SharedEngineCode/Logger.h>
#   include <SharedEngineCode/Internal/PlatformSpecific.h>
#   include <stdlib.h>
#   define BE_INTERFACEFUNCTION(returnType, ...) \
    static returnType (*function)(__VA_ARGS__) = NULL; \
    if (function == NULL) {                      \
        SEC_LOGGER_DEBUG("Getting function from engine: %s\n", __func__); \
        SEC_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(returnType (*)(__VA_ARGS__), function, SEC_PLATFORMSPECIFIC_GET_ADDRESS(I_Engine_GetBinary(), __func__)); \
        if (function == NULL) {                  \
            const char* errorMessage = NULL;     \
            SEC_PLATFORMSPECIFIC_GET_ERROR(errorMessage); \
            SEC_LOGGER_FATAL("An error occurred while getting a function:\n" \
            "Name: %s\n"                         \
            "Reason: %s\n", __func__, errorMessage); \
            abort();                             \
        }                                        \
    }                                            \
    (void) function
#endif
