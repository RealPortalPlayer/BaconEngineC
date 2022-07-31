// Purpose: Allows making launchers easier.
// Created on: 3/28/22 @ 9:54 PM

#pragma once

#include <limits.h>

#include "Internal/CppHeader.h"

CPP_GUARD_START()
    typedef enum {
        LAUNCHER_ERROR_CODE_NULL, // No error.
        LAUNCHER_ERROR_CODE_BINARY, // Errored while opening binary file.
        LAUNCHER_ERROR_CODE_NAME_NULL, // Errored attempting to load the name method.
        LAUNCHER_ERROR_CODE_ENTRY_NULL // Errored attempting to load the entry point.
    } LauncherErrorCodes;

    typedef struct LauncherConfiguration {
        LauncherErrorCodes code;
        union {
            const char* errorMessage; // The message from something like dlopen.
            struct {
                const char* clientName;
                void* clientBinary;
                int (*Start)(struct LauncherConfiguration, int, char**);
            };
        };
    } LauncherConfiguration;

    /**
    * @return The initialized configuration file. NULL if malloc fails.
    */
    void CreateLauncherConfiguration(LauncherConfiguration* configuration, const char* path);
CPP_GUARD_END()