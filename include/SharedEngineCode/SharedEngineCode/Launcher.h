// Purpose: Allows making launchers easier.
// Created on: 3/28/22 @ 9:54 PM

#pragma once

#include <limits.h>

#include "SharedEngineCode/OperatingSystem.h"
#include "Internal/CppHeader.h"

CPP_GUARD_START()
    typedef enum {
        SEC_LAUNCHER_ERROR_CODE_NULL, // No error.
        SEC_LAUNCHER_ERROR_CODE_BINARY, // Errored while opening binary file.
        SEC_LAUNCHER_ERROR_CODE_NAME_NULL, // Errored attempting to load the name method.
        SEC_LAUNCHER_ERROR_CODE_ENTRY_NULL // Errored attempting to load the entry point.
    } SEC_LauncherErrorCodes;

    typedef struct SEC_LauncherConfiguration {
        SEC_LauncherErrorCodes code;
        union {
            const char* errorMessage; // The message from something like dlopen.
            struct {
                const char* clientName;
                void* clientBinary;
                int (*Start)(int, char**);
            };
        };
    } SEC_LauncherConfiguration;

    /**
    * @return The initialized configuration file. NULL if malloc fails.
    */
    void SEC_CreateLauncherConfiguration(SEC_LauncherConfiguration* configuration, const char* path);
CPP_GUARD_END()
